#ifndef __FUNC_LDU_HH__
#define __FUNC_LDU_HH__


#include "BaseFuncUnit.hh"

#include "../Lsq.hh"

#include "../Rcu.hh"

#include "../dCachePort.hh"

namespace Emulator
{
class Func_LDU : public BaseFuncUnit
{
private:

    std::shared_ptr<Lsq>    m_Lsq;

    std::shared_ptr<Rcu>    m_Rcu;

    dCachePort              m_dCachePort;
    uint8_t lsq_count;

public:

    Func_LDU(
        std::shared_ptr<Rcu> rcu,
        std::shared_ptr<Lsq> lsq,
        BaseDRAM*            dram,
        const uint64_t       dCacheLatency,
        const uint16_t FuncUnitId,
        const std::string Name, 
        const uint64_t Latency, 
        const bool Pipelined)
    :   BaseFuncUnit(Name,FuncUnitId,{funcType_t::LDU},Latency,Pipelined),
        m_Rcu(rcu),m_Lsq(lsq),m_dCachePort(dCacheLatency,dram)
    {};
 
    ~Func_LDU(){};

    void Reset(){
        this->m_Allcated = false;
        this->m_CalcuPipe.reset();
        this->m_dCachePort.Reset();
    };

    void AddrGen(InsnPtr_t& insn){
        insn->Agu_addr  =  insn->Operand1 + insn->imm;
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
            if((insn->Agu_addr & 0x1) != 0){
                insn->Excp.valid = false;
                insn->Excp.Cause = RISCV::ExcpCause_t::LD_ADDR_MISALIGNED;
                insn->Excp.Tval  = insn->Agu_addr;
            }
            break;
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
    };

    void Evaluate(){
        if(this->m_CalcuPipe.OutPort->valid){
            this->m_Allcated = false;
            auto& insn = this->m_CalcuPipe.OutPort->data;
            this->AddrGen(insn);//地址产生与异常判断
            this->m_Rcu->AGUFastDetect(lsq_count,insn);
            this->m_Lsq->WriteBack(insn);//将取内存的地址等信息写入lsq队列中，并不是wb stage
        }
        this->SendLoadReq();
        this->m_dCachePort.Evaluate();
    };

    void SendLoadReq(){
        MemReq_t loadReq;
        bool Success = false;
        this->m_Lsq->TryIssueLoad(loadReq,Success);
        if(Success){
            this->m_dCachePort.ReceiveMemReq(loadReq,std::bind(&Func_LDU::ReceiveLoadRespone,this,std::placeholders::_1));
        } 
    }

    void ReceiveLoadRespone(MemResp_t memResp){
        if(memResp.Opcode == MemOp_t::Load){
            auto& ldqEntry = this->m_Lsq->m_LoadQueue[memResp.Id.TransId];//TransId就是指令所在的entry指针位置
            if(!ldqEntry.killed){
                ldqEntry.state = loadState_t::load_Executed;
                auto& insn     = ldqEntry.insnPtr;
                insn->Excp = memResp.Excp;
                if(!memResp.Excp.valid){
                    uint64_t offset = ldqEntry.address & (this->m_Lsq->m_dCacheAlignByte - 1);
                    switch (ldqEntry.insnPtr->SubOp)
                    {
                    case LDU_LB  :
                        insn->RdResult = *(int8_t*)(memResp.Data + offset);
                        break;
                    case LDU_LH  :
                        insn->RdResult = *(int16_t*)(memResp.Data + offset);
                        break;
                    case LDU_LW  :
                        insn->RdResult = *(int32_t*)(memResp.Data + offset);
                        break;
                    case LDU_LD  :
                        insn->RdResult = *(int64_t*)(memResp.Data + offset);
                        break;
                    case LDU_LBU :
                        insn->RdResult = *(uint8_t*)(memResp.Data + offset);
                        break;
                    case LDU_LHU :
                        insn->RdResult = *(uint16_t*)(memResp.Data + offset);
                        break;
                    case LDU_LWU :
                        insn->RdResult = *(uint32_t*)(memResp.Data + offset);
                        break;
                    default:
                        break;
                    }
                }
                bool Success = false;
                for(auto& wbPort : this->m_wbPortVec){
                    if(!wbPort->valid){//如果端口没有需要发送的数据，说明端口空闲
                        wbPort->set(insn);
                        Success = true;
                        insn->State = InsnState_t::State_WriteBack;
                        break;
                    }
                }
                DASSERT(Success,"RobTag[{}],Pc[{}] -> UnAllowed WriteBack Failed");
            }else{
                ldqEntry.state = loadState_t::load_Executed;
            }
        }
    }

    void Advance(){
    
        this->m_CalcuPipe.advance();
        this->m_dCachePort.Advance();
        if((this->m_Pipelined && this->m_Allcated) || this->m_FlushLatch.OutPort->valid){
            this->m_Allcated = false;
        }
    };

};


} // namespace Emulator


#endif