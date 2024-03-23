#ifndef __BASESTAGE_HH__
#define __BASESTAGE_HH__ 

#include "../../Common/Common.hh"
#include "../Pipeline/Pipe_data.hh"
namespace Emulator
{

class Processor;
class BaseStage : public Trace::TraceObject
{
public   : 
    bool                                         m_MustTakenAllInsn   = true;
protected:

    Processor*                                   m_Processor;

    uint64_t                                     m_FromLastStageInsnCount = 0;

    uint64_t                                     m_ToNextStageInsnCount   = 0;

    std::shared_ptr<TimeBuffer<InsnPkg_t>::Port> m_StageInPort;

    std::shared_ptr<TimeBuffer<InsnPkg_t>::Port> m_StageOutPort;

    std::shared_ptr<TimeBuffer<StageAck_t>::Port>m_StageAckInPort;

    std::shared_ptr<TimeBuffer<StageAck_t>::Port>m_StageAckOutPort;

    TimeBuffer<bool>                             m_FlushSyncLatch; 

    std::shared_ptr<TimeBuffer<Redirect_t>::Port>m_RedirectPort;

public:
    BaseStage(Processor* Processor, const std::string Name) :
        TraceObject(Name),
        m_Processor(Processor),
        m_FlushSyncLatch(fmt::format("{}_FlushSyncLatch",Name),1)
    {};

    ~BaseStage(){};

    virtual void Evaluate()        = 0;

    virtual void Reset()           = 0;

    virtual void Advance()           {
        this->m_FlushSyncLatch.advance();
        if(this->m_FlushSyncLatch.OutPort->valid){
            this->FlushAction();
            DPRINTF(Flush,"Flush Stage");
        }
    };

    virtual void ReleaseAction(){};

    virtual void Flush()           {
        this->m_FlushSyncLatch.InPort->set(true);
        this->m_StageOutPort->flush();
    };

    virtual void FlushAction()     {};

    void ConnectStageOutPort(
        std::shared_ptr<TimeBuffer<InsnPkg_t>::Port> NextStageInPort,
        const uint64_t ProcessInsnCount
    ){
        this->m_StageOutPort = NextStageInPort;
        this->m_ToNextStageInsnCount = ProcessInsnCount;
    };

    void ConnectStageInPort(
        std::shared_ptr<TimeBuffer<InsnPkg_t>::Port> LastStageOutPort,
        const uint64_t ProcessInsnCount
    ){
        this->m_StageInPort  = LastStageOutPort;
        this->m_FromLastStageInsnCount = ProcessInsnCount;
    }

    void ConnectStageAckOutPort(
        std::shared_ptr<TimeBuffer<StageAck_t>::Port> StageAckOutPort
    ){
        this->m_StageAckOutPort = StageAckOutPort;
    };

    void ConnectStageAckInPort(
        std::shared_ptr<TimeBuffer<StageAck_t>::Port> StageAckInPort
    ){
        this->m_StageAckInPort = StageAckInPort;
    }

    virtual void ConnectRedirectPort(
        std::shared_ptr<TimeBuffer<Redirect_t>::Port> RedirectPort
    ){
        this->m_RedirectPort  = RedirectPort;
    }

};


} // namespace Emulator





#endif	