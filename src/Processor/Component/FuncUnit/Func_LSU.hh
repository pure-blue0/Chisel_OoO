#ifndef __FUNC_LSU_HH__
#define __FUNC_LSU_HH__


#include "BaseFuncUnit.hh"

#include "../Lsq.hh"

#include "../Rcu.hh"

#include "../dCachePort.hh"
//#include "./obj_dir/VAddrGen.h"
namespace Emulator
{
class Func_LSU : public BaseFuncUnit
{
private:

    std::shared_ptr<Lsq>    m_Lsq;

    std::shared_ptr<Rcu>    m_Rcu;

    dCachePort              m_dCachePort;
    uint8_t lsq_count;

public:

    Func_LSU(
        std::shared_ptr<Rcu> rcu,
        std::shared_ptr<Lsq> lsq,
        BaseDRAM*            dram,
        const uint64_t       dCacheLatency,
        const uint16_t FuncUnitId,
        const std::string Name, 
        const uint64_t Latency, 
        const bool Pipelined)
    :   BaseFuncUnit(Name,FuncUnitId,{funcType_t::LDU,funcType_t::STU},Latency,Pipelined),
        m_Rcu(rcu),m_Lsq(lsq),m_dCachePort(dCacheLatency,dram)
    {};
 
    ~Func_LSU(){};

    void Reset(){
        this->m_Allcated = false;
        this->m_CalcuPipe.reset();
        this->m_dCachePort.Reset();
        lsq_count=0;
    };
    // void AddrGen(InsnPtr_t& insn){
    //     VAddrGen *AddrGen=new VAddrGen;//创建对象
    //     //连接输入
    //     AddrGen->io_Insn_operand1=insn->Operand1;
    //     AddrGen->io_imm=insn->imm;
    //     AddrGen->io_Function_type=insn->Fu;
    //     AddrGen->io_Sub_OP=insn->SubOp;
    //     //运行模块
    //     AddrGen->eval();
    //     //连接输出
    //     insn->Agu_addr=AddrGen->io_Agu_addr;
    //     insn->Agu_addr_ready=AddrGen->io_Agu_addr_ready;
    //     insn->Excp.valid=AddrGen->io_excp_valid;
    //     insn->Excp.Cause=AddrGen->io_excp_cause;
    //     insn->Excp.Tval=AddrGen->io_excpt_tval;
    // }
    void AddrGen(InsnPtr_t& insn){
        insn->Agu_addr  =  insn->Operand1 + insn->imm;
        insn->Agu_addr_ready = true;
        if(insn->Fu == funcType_t::LDU){
            switch (insn->SubOp)
            {
            case LDU_LH  :
                if((insn->Agu_addr & 0b1) != 0){
                    insn->Excp.valid = false;
                    insn->Excp.Cause = RISCV::ExcpCause_t::LD_ADDR_MISALIGNED;
                    insn->Excp.Tval  = insn->Agu_addr;
                }
                break;
            case LDU_LW  :
                if((insn->Agu_addr & 0b11) != 0){
                    insn->Excp.valid = false;
                    insn->Excp.Cause = RISCV::ExcpCause_t::LD_ADDR_MISALIGNED;
                    insn->Excp.Tval  = insn->Agu_addr;
                }
                break;
            case LDU_LD  :
                if((insn->Agu_addr & 0b111) != 0){
                    insn->Excp.valid = false;
                    insn->Excp.Cause = RISCV::ExcpCause_t::LD_ADDR_MISALIGNED;
                    insn->Excp.Tval  = insn->Agu_addr;
                }
                break;
            case LDU_LHU :
                if((insn->Agu_addr & 0b1) != 0){
                    insn->Excp.valid = false;
                    insn->Excp.Cause = RISCV::ExcpCause_t::LD_ADDR_MISALIGNED;
                    insn->Excp.Tval  = insn->Agu_addr;
                }
            case LDU_LWU :
                if((insn->Agu_addr & 0b11) != 0){
                    insn->Excp.valid = false;
                    insn->Excp.Cause = RISCV::ExcpCause_t::LD_ADDR_MISALIGNED;
                    insn->Excp.Tval  = insn->Agu_addr;
                }
                break;
            default:
                break;
            }
        }else if (insn->Fu == funcType_t::STU){
            switch (insn->SubOp)
            {
            case STU_SH:
                if((insn->Agu_addr & 0b1) != 0){
                    insn->Excp.valid = false;
                    insn->Excp.Cause = RISCV::ExcpCause_t::ST_ADDR_MISALIGNED;
                    insn->Excp.Tval  = insn->Agu_addr;
                }
                break;
            case STU_SW:
                if((insn->Agu_addr & 0b11) != 0){
                    insn->Excp.valid = false;
                    insn->Excp.Cause = RISCV::ExcpCause_t::ST_ADDR_MISALIGNED;
                    insn->Excp.Tval  = insn->Agu_addr;
                }
                break;
            case STU_SD:
                if((insn->Agu_addr & 0b111) != 0){
                    insn->Excp.valid = false;
                    insn->Excp.Cause = RISCV::ExcpCause_t::ST_ADDR_MISALIGNED;
                    insn->Excp.Tval  = insn->Agu_addr;
                }
                break;
            default:
                break;
            }
        }
    };
    
    void DataGen(InsnPtr_t& insn){
        insn->Agu_data_ready = true;
        insn->Agu_data = insn->Operand2;
    }

    void Evaluate(){
        
        if(this->m_CalcuPipe.OutPort->valid){
            this->m_Allcated = false;
            auto& insn = this->m_CalcuPipe.OutPort->data;
            this->AddrGen(insn);
            if(insn->Fu == funcType_t::STU){
                insn->Agu_data_ready = true;
                insn->Agu_data = insn->Operand2;
            }
            
            this->m_Rcu->ROB_AGU_EN_Group[this->m_Rcu->lsq_count]=true;
            this->m_Rcu->ROB_AGU_ROBTag_Group[this->m_Rcu->lsq_count]=insn->RobTag;
            
            this->m_Rcu->AGUFastDetect(this->m_Rcu->lsq_count,insn);//检测是否产生了有效的地址和数据，如果指令存在异常，则进行回滚操作
            this->m_Lsq->WriteBack(insn);//更新lsq中对应条目的地址和数据，方便后面的wb
        }
        else this->m_Rcu->ROB_AGU_EN_Group[this->m_Rcu->lsq_count]=false;

        //hdl don't need realize this code
        this->m_Rcu->lsq_count++;
        if(this->m_Rcu->lsq_count==2)this->m_Rcu->lsq_count=0;
        
        bool Success;
        this->SendStoreReq(Success);
        if(!Success){
            this->SendLoadReq();
        }
        this->m_dCachePort.Evaluate();
    };

    void SendStoreReq(bool& Success){
        MemReq_t storeReq;
        storeReq.Data = new char[8];
        Success = false;
        this->m_Lsq->TryIssueStore(storeReq,Success);
        if(Success){
            this->m_dCachePort.ReceiveMemReq(storeReq,std::bind(&Func_LSU::ReceiveRespone,this,std::placeholders::_1));
        } 
        delete[] storeReq.Data;
    }

    void SendLoadReq(){
        MemReq_t loadReq;
        bool Success = false;
        this->m_Lsq->TryIssueLoad(loadReq,Success);
        if(Success){
            this->m_dCachePort.ReceiveMemReq(loadReq,std::bind(&Func_LSU::ReceiveRespone,this,std::placeholders::_1));
        } 
    }

    void ReceiveRespone(MemResp_t memResp){
        if(memResp.Opcode == MemOp_t::Load){
            auto& ldqEntry = this->m_Lsq->m_LoadQueue[memResp.Id.TransId];
            if(!ldqEntry.killed){
                ldqEntry.state = loadState_t::load_Executed;
                
                InsnPtr_t& insn     = ldqEntry.insnPtr;
                insn->BruMisPred=false;
                insn->Fu=ldqEntry.Fu;
                insn->RobTag=ldqEntry.RobTag;
                insn->IsaRd=ldqEntry.IsaRd;
                insn->PhyRd=ldqEntry.PhyRd;
                insn->Excp = memResp.Excp;
                if(!memResp.Excp.valid){
                    uint64_t offset = ldqEntry.address & (this->m_Lsq->m_dCacheAlignByte - 1);
                    switch (ldqEntry.insnPtr->SubOp)
                    {
                    case LDU_LB  :insn->RdResult = *(int8_t*)(memResp.Data + offset);break;
                    case LDU_LH  :insn->RdResult = *(int16_t*)(memResp.Data + offset);break;
                    case LDU_LW  :insn->RdResult = *(int32_t*)(memResp.Data + offset);break;
                    case LDU_LD  :insn->RdResult = *(int64_t*)(memResp.Data + offset);break;
                    case LDU_LBU :insn->RdResult = *(uint8_t*)(memResp.Data + offset);break;
                    case LDU_LHU :insn->RdResult = *(uint16_t*)(memResp.Data + offset);break;
                    case LDU_LWU :insn->RdResult = *(uint32_t*)(memResp.Data + offset);break;
                    default:insn->RdResult =0;break;
                    }
                }
                for(auto& wbPort : this->m_wbPortVec){
                    if(!wbPort->valid){
                        wbPort->set(insn);
                        insn->State = InsnState_t::State_WriteBack;
                        break;
                    }
                }
            }else{
                ldqEntry.state = loadState_t::load_Executed;
            }
        }else if(memResp.Opcode == MemOp_t::Store){
            auto& stqEntry = this->m_Lsq->m_StoreQueue[memResp.Id.TransId];
            stqEntry.state = storeState_t::store_Executed;
            this->m_Lsq->m_StoreQueue.Pop();
        }
    }

    void Advance(){
        this->m_FlushLatch.advance();
        this->m_CalcuPipe.advance();
        this->m_dCachePort.Advance();
        if((this->m_Pipelined && this->m_Allcated) || this->m_FlushLatch.OutPort->valid){
            this->m_Allcated = false;
        }
    };

};


} // namespace Emulator


#endif