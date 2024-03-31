#ifndef __BASEFUNCUNIT_HH__
#define __BASEFUNCUNIT_HH__


#include "../../../Timing/TimeBuffer.hh"
#include "../../../Trace/TraceObject.hh"
#include "../../Pipeline/Pipe_data.hh"
#include <vector>
#include <set>

namespace Emulator
{
class BaseFuncUnit : public Trace::TraceObject
{
protected:

    const bool              m_Pipelined;

    TimeBuffer<InsnPtr_t>   m_CalcuPipe;

    TimeBuffer<bool>        m_FlushLatch;

    std::vector<std::shared_ptr<TimeBuffer<InsnPtr_t>::Port>> m_wbPortVec;

    bool                    m_Allcated;

public:

    const std::set<funcType_t>    m_SupportFunc;

    const uint16_t          m_FuncUnitId;

public:
    
    BaseFuncUnit(
        const std::string Name, 
        const uint16_t FuncUnitId,
        const std::set<funcType_t> SupportFunc, 
        const uint64_t Latency, 
        const bool Pipelined
    ):  TraceObject(Name),
        m_CalcuPipe(fmt::format("{}Pipe",Name),Latency),
        m_FlushLatch(fmt::format("{}FlushLatch",Name),1),
        m_FuncUnitId(FuncUnitId),
        m_SupportFunc(SupportFunc),
        m_Pipelined(Pipelined)
    {
        if(!(Latency > 0)){
            SPDLOG_ERROR("Latency Should Greater then 1, Given FuncUnit[{}] Latency[{}]",Name,Latency);
        }
    };

    ~BaseFuncUnit(){};

    virtual void Reset(){
        this->m_Allcated = false;
        this->m_CalcuPipe.reset();
    };

    virtual void Flush(){
        this->m_FlushLatch.InPort->set(true);
        this->m_CalcuPipe.flush();
    };

    virtual bool Busy(){
        if(this->m_Pipelined){
            return this->m_CalcuPipe.isStalled() || this->m_Allcated ;
        }else{
            return !this->m_CalcuPipe.isDrained() || this->m_CalcuPipe.isStalled() || this->m_Allcated;
        }
    };

    virtual void Allocate(){
        DASSERT((this->m_Pipelined || !this->m_Allcated), "Already Allocated!");
        this->m_Allcated = true;
    };

    virtual void AddWbPort(std::shared_ptr<TimeBuffer<InsnPtr_t>::Port> wbPort){
        this->m_wbPortVec.emplace_back(wbPort);
    };

    virtual void Evaluate(){
        if(this->m_CalcuPipe.OutPort->valid){
            auto& insn = this->m_CalcuPipe.OutPort->data;
            bool  Success = false;
            this->Compute(insn);
            for(auto& wbPort : this->m_wbPortVec){
                if(!wbPort->valid){
                    wbPort->set(insn);
                    Success = true;
                    insn->State = InsnState_t::State_WriteBack;
                    this->m_Allcated = false;//释放功能单元
                    break;
                }
            }
            if(!Success){
                this->m_CalcuPipe.stall();
                DPRINTF(Stall,"RobTag[{}],Pc[{:#x}] -> Write Back Failed, Stall Pipeline",insn->RobTag,insn->Pc);
            }
        }
    };

    virtual void Advance(){
        this->m_FlushLatch.advance();
        this->m_CalcuPipe.advance();
        if((this->m_Pipelined && this->m_Allcated) || this->m_FlushLatch.OutPort->valid){
            this->m_Allcated = false;
        }
    };

    virtual void Issue(InsnPtr_t& insn){
        DASSERT((this->m_SupportFunc.count(insn->Fu)), "Unsupported Function Type");
        DASSERT((!this->m_CalcuPipe.InPort->valid), "FuncUnit has been Issued a Instruction this Cycle !")
        this->m_Allcated = true;
        this->m_CalcuPipe.InPort->set(insn);
        insn->State = InsnState_t::State_Execute;
        DPRINTF(Execute,"RobTag[{}],Pc[{:#x}] -> Get Insn, Start Executing",insn->RobTag,insn->Pc);
    };


    virtual void Compute(InsnPtr_t& insn){
        DERROR("Didn't Implement!");
    };
};



} // namespace Emulator





#endif