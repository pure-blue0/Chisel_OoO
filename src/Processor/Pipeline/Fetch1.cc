#include "Fetch1.hh"
#include "../Processor.hh"
#include "../../Simulator.hh"
namespace Emulator
{

Fetch1::Fetch1(
    Processor*                       processor              ,
    const uint64_t                   iCacheLatency          ,
    const uint64_t                   iCacheAlignByte        ,
    const uint64_t                   FetchByteWidth         ,
    const uint64_t                   InflightQueueSize 
) : BaseStage(processor,"Fetch1"),
    m_PcRegister("PcRegister",1),
    m_InflightQueue("FetchInflightQueue",InflightQueueSize),
    m_iCacheAlignByte(iCacheAlignByte),
    m_iCachePort(iCacheLatency,this->m_Processor->getDramPtr()),
    m_FetchByteWidth(FetchByteWidth),
    m_PredSync(FetchByteWidth/2)
{
}

Fetch1::~Fetch1(){}

void 
Fetch1::Evaluate(){
    static bool full;
    //--------------------------------------------------------------//
    bool SendSuccess=false;
    MemReq_t fetchReq;
    InflighQueueEntry_t NewEntry(this->m_FetchByteWidth);
    NewEntry.Excp.valid=false;
    //module 1
    this->SendFetchReq(full,this->m_InflightQueue_tail,SendSuccess,fetchReq,NewEntry);//处理pcregister的输出端口数据，并将其放入m_InflightQueue
    //module 2
    this->m_iCachePort.ReceiveFetchReq(fetchReq,SendSuccess);
    //-------------------------------------------------------------//
    MemResp_t resp;
    bool resp_valid;
    //module 3
    this->m_iCachePort.Evaluate(resp,resp_valid);//输出icache中缓存的数据（通过调用fetch1的receivereq函数，参数是上一拍从内存中获取到的mem数据）

    //module 4 InflightQueue   
    this->InflightQueue_Evaluate(true,this->fetchQueue_flush,SendSuccess,resp_valid,NewEntry,resp,this->m_InflightQueue_tail,full);
    //-------------------------------------------------------------//
    //module 5 
    this->GenNextFetchAddress(SendSuccess);

    if(NewEntry.Excp.valid){
        this->m_ExcpTag     = fetchReq.Id.TransId;
        this->m_State       = State_t::HandleExcp;
    }
}
void Fetch1::InflightQueue_Evaluate(bool reset_n,bool fetchQueue_flush,bool SendSuccess,bool mem_valid,InflighQueueEntry_t EntryData,MemResp_t mem,uint8_t& r_tail_ptr,bool& full){
    static InflighQueueEntry_t FetchQueue[FetchQueue_Size];
    static uint8_t header_ptr,tail_ptr;
    static uint8_t usage_count;
    bool empty;
    InsnPkg_t insnPkg;
    auto insn = this->m_Processor->CreateInsn();

    if(usage_count==0)empty=true;
    else empty=false;

    if(!reset_n||fetchQueue_flush)
    {
        FetchQueue->reset(this->m_FetchByteWidth);
        usage_count=0;
        header_ptr=0;
        tail_ptr=0;
        empty=true;
        full=false;
        insn->data_valid=false;
    }
    else{
        if(!empty){
            if(!FetchQueue[header_ptr].Busy&&!FetchQueue[header_ptr].Killed){
                insn->data_valid=true;
                insn->Address   = FetchQueue[header_ptr].Address;
                insn->Excp = FetchQueue[header_ptr].Excp;
                insn->InsnByte=FetchQueue[header_ptr].InsnByte;
            }
            else{
                insn->data_valid=false;
            }

            if(!FetchQueue[header_ptr].Busy){
                header_ptr++;
                if(header_ptr==FetchQueue_Size)header_ptr=0;
                usage_count--;
            }
        }
        else{
            insn->data_valid=false;
        }

        if(SendSuccess||EntryData.Excp.valid){
            FetchQueue[tail_ptr] = EntryData;
            usage_count++;
            tail_ptr++;
            if(tail_ptr==FetchQueue_Size)tail_ptr=0;
        }
        
        if(mem_valid){
            FetchQueue[mem.Id.TransId].Busy = false;
            FetchQueue[mem.Id.TransId].Excp = mem.Excp;
            memcpy(FetchQueue[mem.Id.TransId].InsnByte.data(),mem.Data,this->m_FetchByteWidth);
        }
    }

    insnPkg.emplace_back(insn);
    this->m_StageOutPort->set(insnPkg);  
    r_tail_ptr=tail_ptr;
    if(usage_count==FetchQueue_Size)full=true;
    else full=false;
}
void
Fetch1::SendFetchReq(bool full,uint8_t InflightQueue_tail,bool& SendSuccess,MemReq_t& fetchReq,InflighQueueEntry_t& NewEntry){
    SendSuccess = false;
    NewEntry.Killed     = false;
    NewEntry.Address    = this->m_PcRegister.OutPort->data;
    fetchReq.Id.TransId = InflightQueue_tail;// 获取尾指针
    if(this->m_PcRegister.OutPort->valid ){    
        /* Check Alignment */
        if(!full && this->m_State == State_t::Idle && 
            !this->m_StageOutPort->isStalled() )//不满，空闲，不堵塞
        {
            if((this->m_PcRegister.OutPort->data & 0b1) == 0 ){//0b1:二进制的1，只是看最低位是否为0，如果不是则触发异常
                NewEntry.Busy       = true;//表示当前数据未经过receivereq处理
                fetchReq.Address    = this->m_PcRegister.OutPort->data & ~(m_FetchByteWidth - 1);
                fetchReq.Length     = this->m_FetchByteWidth;
                fetchReq.Opcode     = MemOp_t::Fetch;
                NewEntry.Excp.valid = false;
                SendSuccess = true;   
            }else{
                NewEntry.Excp.valid = true;
                NewEntry.Excp.Cause = RISCV::INSTR_ADDR_MISALIGNED;
                NewEntry.Excp.Tval  = this->m_PcRegister.OutPort->data;
                NewEntry.Busy       = false;
            }
        }
    }
}

void 
Fetch1::Advance(){
    this->m_PcRegister.advance();
    this->m_iCachePort.Advance();
    this->m_FlushSyncLatch.advance();
    this->Decode_Redirect_Reg->advance();
    this->IEW_Redirect_Reg->advance();
    this->Commit_Redirect_Reg->advance();
    if(this->m_FlushSyncLatch.OutPort->valid){
        this->FlushAction();
        DPRINTF(Flush,"Flush Stage");
        
    }
}

void 
Fetch1::Reset(){
    this->m_MisalignValid    = false;
    this->m_MisalignHalf     = 0;
    this->m_ExcpTag          = 0;
    this->m_State            = State_t::Idle;
    this->m_iCachePort.Reset();
   
    this->m_FlushSyncLatch.reset();
    this->m_PcRegister.reset();
    this->Decode_Redirect_Reg->reset();
    this->IEW_Redirect_Reg->reset();
    this->Commit_Redirect_Reg->reset();
}   

//Function：
//生成下一个字节的取指地址，优先级为 backend redirect > predicted PC > PC + 4，其中越后端的stage发出的重定向请求，优先级越高。
void Fetch1::GenNextFetchAddress(bool SendSuccess){
    if(this->Commit_Redirect_Reg->OutPort->data.valid){
        this->m_PcRegister.InPort->set(this->Commit_Redirect_Reg->OutPort->data.target);
    }
    else if(this->IEW_Redirect_Reg->OutPort->data.valid){
        this->m_PcRegister.InPort->set(this->IEW_Redirect_Reg->OutPort->data.target);
    }
    else if(this->Decode_Redirect_Reg->OutPort->data.valid){
        this->m_PcRegister.InPort->set(this->Decode_Redirect_Reg->OutPort->data.target);
    }
    else if(SendSuccess){
        this->m_PcRegister.InPort->set((this->m_PcRegister.OutPort->data & ~(this->m_iCacheAlignByte - 1)) + this->m_FetchByteWidth);
    }
    else if(this->m_PcRegister.OutPort->valid){
        this->m_PcRegister.InPort->set(this->m_PcRegister.OutPort->data);
    }
}


void 
Fetch1::InitBootPc(Addr_t boot_address){
    this->m_PcRegister.InPort->set(boot_address);
}
void 
Fetch1::FlushAction(){
    this->fetchQueue_flush=true;
    // this->m_iCachePort.Reset();
    this->m_PcRegister.OutPort->kill();
    this->m_MisalignValid    = false;
    this->m_MisalignHalf     = 0;
    this->m_ExcpTag          = 0;
    this->m_State            = State_t::Idle;
}

void 
Fetch1::KillNewer(uint64_t InflightQueueTag){
    for(auto ptr = InflightQueueTag; ptr != this->m_InflightQueue.getTail();ptr = this->m_InflightQueue.getNextPtr(ptr)){
        InflighQueueEntry_t& entry = this->m_InflightQueue[ptr];
        entry.Killed = true;
    }
}



void 
Fetch1::AddRedirectPort(std::shared_ptr<TimeBuffer<Redirect_t>::Port> RedirectPort){
    this->m_RedirectPortVec.emplace_back(RedirectPort);
}


std::shared_ptr<BaseStage> Create_Fetch1_Instance(
        Processor*                       processor         ,
        const YAML::Node&                StageConfig       )
{
    const uint64_t iCacheLatency        = processor->m_iCacheLatency;
    const uint64_t iCacheAlignByte      = processor->m_iCacheAlignByte ;
    const uint64_t FetchByteWidth       = StageConfig["FetchByteWidth"].as<uint64_t>() ;
    const uint64_t InflightQueueSize    = StageConfig["InflightQueueSize"].as<uint64_t>() ;
    return std::shared_ptr<BaseStage>(
        new Fetch1(processor,iCacheLatency,iCacheAlignByte,FetchByteWidth,InflightQueueSize)
    );

}

} // namespace Emulator
