#ifndef __BASESCHEDULAR_HH__
#define __BASESCHEDULAR_HH__

#include "../../../Trace/TraceObject.hh"
#include "../../Pipeline/Pipe_data.hh"
#include "../FuncUnit/BaseFuncUnit.hh"
#include "../FreeList.hh"
#include "../Rcu.hh"
#include <deque>
#include <set>

namespace Emulator
{

enum IssuePolicy_t{
    InOrder,OutofOrder,InAgeOrder
};

struct IssueInfo{
    bool      isToFu; // 1 : To fu , 0 : To subSchedular
    uint16_t  desIndex;
    uint64_t  entryIndex; // Only for subSchedular
    InsnPtr_t insn;
};

struct SchedualInfo_t{
    uint64_t    entryIndex;
    InsnPtr_t   insn;
};

class BaseSchedular : public Trace::TraceObject
{
public:
    const uint64_t                              m_EnqueWidth;

    const uint64_t                              m_DequeWidth;
    
protected:

    // TimeBuffer<SchedualInfo_t>                  m_EnqueLatch;

    TimeBuffer<std::deque<SchedualInfo_t>>      m_EnqueLatch;

    TimeBuffer<bool>                            m_FlushLatch;

    std::shared_ptr<Rcu>                        m_Rcu;

    const uint64_t                              m_EntryCount;

    std::vector<std::shared_ptr<BaseFuncUnit>>  m_FuncUnitVec;

    std::vector<std::shared_ptr<BaseSchedular>> m_SubSchedular;

    std::vector<std::shared_ptr<TimeBuffer<IssueInfo>::Port>>   m_RFReadPortVec;

    uint64_t                                    m_AllocatedCount;

public:

    const uint16_t                              m_SchedularId;

    std::set<funcType_t>                        m_SupportFunc;

public:
    
    BaseSchedular(
        const std::string    Name, 
        const uint16_t       SchedularId,
        const uint64_t       EntryCount,
        const uint64_t       EnqueWidth,
        const uint64_t       DequeWidth,
        std::shared_ptr<Rcu> rcu
    ):  TraceObject(fmt::format("{}",Name)),
        m_EnqueLatch(fmt::format("{}_EnqueueLatch",Name),1),
        m_FlushLatch(fmt::format("{}_FlushLatch",Name),1),
        m_SchedularId(SchedularId),
        m_EntryCount(EntryCount),
        m_EnqueWidth(EnqueWidth),
        m_DequeWidth(DequeWidth),
        m_Rcu(rcu)
    {
    };

    ~BaseSchedular(){};

    virtual void Reset() = 0;

    void Flush(){
        this->m_FlushLatch.InPort->set(true);
        this->m_EnqueLatch.InPort->data.clear();
        this->m_EnqueLatch.flush();
    }

    virtual bool Busy() {
        return this->IssueQueueFull() || this->m_AllocatedCount == 0;
    };

    virtual bool IssueQueueFull()  = 0;

    virtual bool Empty() = 0;

    virtual uint64_t GetAvailibleEntryCount()  = 0;

    virtual uint64_t Allocate(){
        DASSERT((this->m_AllocatedCount>0),"No Availible Port");
        uint64_t Entryindex = this->AllocateEntry();
        this->m_AllocatedCount--;
        return Entryindex;
    };

    virtual uint64_t AllocateEntry() = 0;

    virtual void Schedule(InsnPtr_t& insn, uint64_t index){
        this->m_EnqueLatch.InPort->valid = true;
        this->m_EnqueLatch.InPort->data.push_back({index,insn});
        DPRINTF(Issue,"RobTag[{}],Pc[{:#x}] -> Enter Queue",insn->RobTag,insn->Pc);
    }

    virtual void Dispatch(InsnPtr_t& insn, uint64_t& index) = 0;

    virtual void IssueSelect(uint64_t& index, InsnPtr_t& insn, bool& Success) = 0;

    virtual void Deallocate(uint64_t& index) = 0;

    virtual void Forwarding(InsnPtr_t& insn){};

    void Evaluate(){
        // if(this->m_EnqueLatch.OutPort->valid){
        //     auto& t = this->m_EnqueLatch.OutPort->data;
        //     t.insn->State = InsnState_t::State_Issue;
        //     this->Dispatch(t.insn,t.entryIndex);
        // }
        for(size_t i = 0 ; i < this->m_DequeWidth; i++){
            if(!this->Empty()){
                uint64_t    index;
                InsnPtr_t   insn;
                bool        Success;
                this->IssueSelect(index,insn,Success);
                if(Success){
                    this->Deallocate(index);
                    DPRINTF(Issue,"RobTag[{}],Pc[{:#x}] -> Leaving Queue",insn->RobTag,insn->Pc);
                }else{
                    break;
                }
            }else{
                break;
            }
        }
    };

    void Advance(){
        this->m_AllocatedCount = this->m_EnqueWidth;
        this->m_EnqueLatch.advance();
        this->m_EnqueLatch.InPort->data.clear();
        this->m_FlushLatch.advance();

        if(this->m_EnqueLatch.OutPort->valid){
            auto& info_queue = this->m_EnqueLatch.OutPort->data;
            for(auto& info : info_queue){
                info.insn->State = InsnState_t::State_Issue;
                this->Dispatch(info.insn,info.entryIndex);
            }
            info_queue.clear();
        }
        if(this->m_FlushLatch.OutPort->valid){
            this->Reset();
        }
    };

    void AddRFReadPort(std::shared_ptr<TimeBuffer<IssueInfo>::Port> RfReadPort){
        this->m_RFReadPortVec.push_back(RfReadPort);
    };

    void AddFuncUnit(std::shared_ptr<BaseFuncUnit> funcUnit){
        for(auto fuType : funcUnit->m_SupportFunc){
            if(!this->m_SupportFunc.count(fuType)){
                this->m_SupportFunc.insert(fuType);
            }
        }
        this->m_FuncUnitVec.emplace_back(funcUnit);
    };

    void AddSubSchedular(std::shared_ptr<BaseSchedular> subSchedular){
        for(auto fuType : subSchedular->m_SupportFunc){
            if(!this->m_SupportFunc.count(fuType)){
                this->m_SupportFunc.insert(fuType);
            }
        }
        this->m_SubSchedular.emplace_back(subSchedular);
    };

};

    
} // namespace Emulator


#endif