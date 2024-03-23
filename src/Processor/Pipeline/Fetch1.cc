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
{}

Fetch1::~Fetch1(){}

void 
Fetch1::Evaluate(){
    this->m_iCachePort.Evaluate();//输出icache中缓存的数据（通过调用fetch1的receivereq函数，参数是上一拍从内存中获取到的mem数据）
    this->SendReq();
    
    bool SendSuccess;
    if(this->m_PcRegister.OutPort->valid){
        this->SendFetchReq(SendSuccess);//处理pcregister的输出端口数据，并将其放入m_InflightQueue《后续直接放到Fetch1中实现》
        this->Predict(this->m_PcRegister.InPort->data,this->m_PredSync);//处理pcregister的输入端口数据，默认不跳转
    }
    this->GenNextFetchAddress(SendSuccess);
}

void 
Fetch1::Advance(){
    this->m_PcRegister.advance();
    this->m_iCachePort.Advance();
    this->m_FlushSyncLatch.advance();
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
    this->m_InflightQueue.Reset();
    this->m_FlushSyncLatch.reset();
    this->m_PcRegister.reset();
}


void 
Fetch1::InitBootPc(Addr_t boot_address){
    this->m_PcRegister.InPort->set(boot_address);
}

void
Fetch1::GenNextFetchAddress(bool& SendSuccess){
    InsnState_t redirectTag = InsnState_t::State_Fetch1;
    if(this->m_PcRegister.OutPort->valid){
        this->m_PcRegister.InPort->set(this->m_PcRegister.OutPort->data);
    }
    if(SendSuccess){
        this->m_PcRegister.InPort->set((this->m_PcRegister.OutPort->data & ~(this->m_iCacheAlignByte - 1)) + this->m_FetchByteWidth);
    }
    for(auto RedirectPort : this->m_RedirectPortVec){
        //如果某个重定向端口的数据有效且所在阶段大于或等于 Fetch1，则表示有更高优先级的重定向请求,越往后优先级越高
        if(RedirectPort->valid && RedirectPort->data.StageId >= redirectTag){
            this->m_PcRegister.InPort->set(RedirectPort->data.target);
            redirectTag = RedirectPort->data.StageId;
        }
    }
}

void 
Fetch1::Predict(Addr_t& Pc,std::vector<Pred_t>& insnPred){
    for(auto& pred : insnPred){
        pred.taken_valid  = false;//说明预测是否有效
        pred.taken        = false;//预测结果
        pred.target       = 0;//预测地址
        pred.target_valid = false;//预测地址是否有效
    }
}

void
Fetch1::SendFetchReq(bool& SendSuccess){
    MemReq_t            fetchReq;
    InflighQueueEntry_t NewEntry(this->m_FetchByteWidth);
    SendSuccess = false;
    /* Check Alignment */
    if(!this->m_InflightQueue.full() && this->m_State == State_t::Idle && 
        !this->m_StageOutPort->isStalled() )//不满，空闲，不堵塞
    {
        NewEntry.Address    = this->m_PcRegister.OutPort->data;
        NewEntry.Killed     = false;
        //NewEntry.InsnPred   = this->m_PredSync;
        fetchReq.Id.TransId = this->m_InflightQueue.Allocate();// 获取尾指针
        if((this->m_PcRegister.OutPort->data & 0b1) == 0 ){//0b1:二进制的1，只是看最低位是否为0，如果不是则触发异常
            NewEntry.Busy       = true;//表示当前数据未经过receivereq处理
            fetchReq.Address    = this->m_PcRegister.OutPort->data & ~(m_FetchByteWidth - 1);
            fetchReq.Length     = this->m_FetchByteWidth;
            fetchReq.Opcode     = MemOp_t::Fetch;
            SendSuccess = true;
            this->m_InflightQueue[fetchReq.Id.TransId] = NewEntry;
            this->m_iCachePort.ReceiveFetchReq(fetchReq,std::bind(&Fetch1::ReceiveReq,this,std::placeholders::_1));
        }else{
            NewEntry.Excp.valid = true;
            NewEntry.Excp.Cause = RISCV::INSTR_ADDR_MISALIGNED;
            NewEntry.Excp.Tval  = this->m_PcRegister.OutPort->data;
            NewEntry.Busy       = false;
            this->m_InflightQueue[fetchReq.Id.TransId] = NewEntry;
            this->m_State       = State_t::HandleExcp;
            this->m_ExcpTag     = fetchReq.Id.TransId;
        }
    }
}

void 
Fetch1::FlushAction(){
    this->KillNewer(this->m_InflightQueue.getHeader());
    // this->m_iCachePort.Reset();
    // this->m_InflightQueue.Reset();
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
Fetch1::ReceiveReq(MemResp_t mem_resp){
    
    InflighQueueEntry_t& entry = this->m_InflightQueue[mem_resp.Id.TransId];//储存当前条目的指针，尾指针-1
    entry.Busy = false;
    entry.Excp = mem_resp.Excp;
    memcpy(entry.InsnByte.data(),mem_resp.Data,this->m_FetchByteWidth);

#ifdef TRACE_ON
    std::stringstream insnByte;
    for(size_t i = 0; !mem_resp.Excp.valid && i < this->m_FetchByteWidth; i++){
        insnByte << fmt::format("{:02x} ",*(uint8_t*)(mem_resp.Data+(m_FetchByteWidth-1-i)));
    }
    DPRINTF(ICacheResp,"Address : {:#x}, Killed : {}, Excp : {} \n\tFetch Package {}",
                mem_resp.Address,entry.Killed,mem_resp.Excp.valid,insnByte.str());
#endif

}

void 
Fetch1::Predecode(InflighQueueEntry_t& frontEntry ,InsnPkg_t& insnPkg){
    bool needRedirect;
    Redirect_t RedirectReq;
    //最旧的那条异常指令也会进入预解码，不会被kill
    DASSERT(!frontEntry.Killed,"Killed But send to Next Stage!");
    if(frontEntry.Excp.valid){//在这里只有前面出现INSTR_ADDR_MISALIGNED才会导致这种可能
        auto insn = this->m_Processor->CreateInsn();
        insn->Pc   = frontEntry.Address;//就只发送一条异常的地址
        insn->Excp = frontEntry.Excp;
        this->m_State = State_t::WaitForResume;
        insnPkg.emplace_back(insn);
    }else{
        uint64_t offset  = frontEntry.Address & (this->m_iCacheAlignByte-1); //看当前的地址是否对齐，也就是低五位是否为0
       // DASSERT(!(offset && this->m_MisalignValid),"Shift Cacheline when MisAlign valid!");//避免出现上一次的地址发生截断，还不连续的情况
        //如果当前数据的地址并不是对其的，比如0x80000010，偏移了16位，那么头指针也需要偏移16位，然后再开始取指令
        char* dataPtr = frontEntry.InsnByte.data() + offset;
        uint64_t numByte = m_FetchByteWidth - 
                (frontEntry.Address & (this->m_iCacheAlignByte-1))  ;//32-偏移的16位，然后如果上一次最后的指令发送了截断，那就多处理两bit
                                                //因为如果发送misalign，那么一行指令的一半在上一行的末尾，另一半在这一行的开头
        uint64_t Npc  = frontEntry.Address ;//偏移到上一行的倒数第二个字节

        while(numByte){
            InsnPtr_t insn          = this->m_Processor->CreateInsn();//调用processor的函数是因为这里只是填加了insn的pc，后面其他stage还要添加信息
            insn->Pc                = Npc;
            insn->Excp              = frontEntry.Excp;
            //判断是否是压缩指令，如果上一次发生截断，这一次的头两个数据只能是剩下的那两byte高位地址数据，那么就把当前数据左移16位，和上一次的拼起来
           // insn->IsRvcInsn         = false;
            insn->UncompressedInsn    = *(uint32_t*)dataPtr;
            insnPkg.emplace_back(insn);
            this->BranchRedirect(insn,frontEntry.InsnPred[(offset >> 1)],needRedirect,RedirectReq);
            if(needRedirect){//如果需要重定向，那么后面的数据也就没必要放进来了
                DPRINTF(Redirect,"Pc[{:#x}] -> Predict Mismatch, Redirect to {:#x}",insn->Pc,RedirectReq.target);
                this->m_FlushSyncLatch.InPort->set(true);
                this->m_RedirectPort->set(RedirectReq);
                break;
            }
            numByte -= 4;
            dataPtr += 4;
            Npc     += 4;
            offset  += 4;
        }   
    }     
}

void
Fetch1::BranchRedirect(InsnPtr_t& insn, Pred_t& Pred, bool& needRedirect,Redirect_t& RedirectReq){
    needRedirect = false;
    RedirectReq.StageId = InsnState_t::State_Fetch1;
    RISCV::StaticInsn instruction(insn->UncompressedInsn);//对指令进行解码
    insn->Pred.Taken        = false;
    insn->Pred.Target       = insn->Pc +  4;//pred.target内保存的是下一条地址，具体的地址取决于是否跳转
    if(instruction.opcode() == 0b1101111){ // JAL//默认跳转
        insn->Pred.Taken    = true;
        insn->Pred.Target   = insn->Pc + instruction.ujimm();
        if(!(Pred.taken_valid == true && Pred.taken == true && Pred.target_valid && Pred.target == (insn->Pc + instruction.ujimm()))){
            //如果以上所有条件都成立，说明预测的跳转目标地址与 JAL 指令的跳转目标地址一致，此时不需要重定向
            needRedirect = true;
            RedirectReq.target = insn->Pred.Target;
        }
    }else if(instruction.opcode() == 0b1100111 && instruction.func3() == 0b000){ // JALR//默认不跳转
        if(Pred.taken_valid && Pred.target_valid){
            insn->Pred.Taken  = Pred.taken;
            insn->Pred.Target = Pred.target;
        }
    }else if(instruction.opcode() == 0b1100011){ // BRANCH//默认不跳转
        if(Pred.taken_valid && Pred.taken){
            insn->Pred.Taken = true;
            insn->Pred.Target = insn->Pc + instruction.sbimm();
            if(!(Pred.target_valid && Pred.target == (insn->Pc + instruction.sbimm()))){
                needRedirect = true;
                RedirectReq.target = insn->Pred.Target;
            }
        }
    }
    else{
        if(Pred.taken_valid && Pred.target_valid){
            needRedirect = true;
            RedirectReq.target = insn->Pred.Target; 
        }
    }

}

void
Fetch1::SendReq(){
    InflighQueueEntry_t& frontEntry = this->m_InflightQueue.front();
    if(!this->m_InflightQueue.empty() && !frontEntry.Busy){//队列非空，且frontentry已经获取了从cache返回的数据
        if(!frontEntry.Killed){//如果frontentry没有被kill（只有异常处理时，才有可能（是可能，不是一定））
            InsnPkg_t insnPkg;
            this->Predecode(frontEntry,insnPkg);
            if(insnPkg.size()){//如果预解码出数据了，则把数据送到输出端口
                this->m_StageOutPort->set(insnPkg);
            }
        }
        this->m_InflightQueue.Pop();//从队列中弹出frontentry
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
