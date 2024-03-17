#ifndef __PROCESSOR_HH__
#define __PROCESSOR_HH__ 

#include "../Common/Common.hh"
#include "StageFactory.hh"
#include "DynInsn.hh"
#include "Component/Rcu.hh"
#include "Component/RfReadPort.hh"
#include "Component/WriteBackPort.hh"
#include "Component/FuncUnit/FuncUnitFactory.hh"
#include "Component/Schedular/SchedularFactory.hh"
#include "../Memory/BaseDRAM.hh"
#include "Pipeline/Fetch1.hh"
#include "Component/Lsq.hh"

#include <yaml-cpp/yaml.h>
#include <map>
#include <deque>
#include <memory>

namespace Emulator
{


class Simulator;
class Processor : public Trace::TraceObject
{
public:

    /* Config */
    ThreadId                m_tid;

    uint8_t                 m_XLEN;

    std::set<std::string>   m_SupportISA;

    std::set<std::string>   m_SupportMode;

    bool                    m_doSpeculation;

    bool                    m_forwardResult;

    std::string             m_MemoryModel;

    std::shared_ptr<RISCV::ExecContext>   m_ExecContext;

    /* iCache Config */
    uint64_t                m_iCacheLatency;
    uint64_t                m_iCacheAlignByte;
    /* dCache Config */
    uint64_t                m_dCacheLatency;
    uint64_t                m_dCacheAlignByte;

private:

    BaseDRAM*               m_DramPtr;
    
    /* Instruction Pool */
    std::deque<std::weak_ptr<DynInsn>>    m_InsnPool;

    /* PipeLine */

    std::map<std::string,std::shared_ptr<BaseStage>>    m_StageMap;

    std::deque<std::shared_ptr<TimeBuffer<InsnPkg_t>>>  m_ForwardConnectionLatch;

    std::deque<std::shared_ptr<TimeBuffer<StageAck_t>>> m_BackwardConnectionLatch;

    std::deque<std::shared_ptr<TimeBuffer<Redirect_t>>> m_RedirectConnectionLatch;

    std::deque<std::pair<InsnState_t,std::shared_ptr<BaseStage>>> m_StageTickSeq;

    std::shared_ptr<Rcu>                                m_Rcu;
    
    std::shared_ptr<Lsq>                                m_Lsq;

public:


    std::vector<std::shared_ptr<BaseFuncUnit>>          m_FuncUnitVec;

    std::vector<std::shared_ptr<BaseSchedular>>         m_SchedularVec;

    std::vector<std::shared_ptr<RfReadPort>>            m_RfReadPortVec;

    std::vector<std::shared_ptr<WriteBackPort>>         m_WriteBackPortVec;

public:

    Processor(const YAML::Node& ProcessorConfig, BaseDRAM* Dram ,const ThreadId tid);


    BaseDRAM* getDramPtr();

    std::shared_ptr<Rcu>      getRcuPtr();

    std::shared_ptr<Lsq>      getLsqPtr();

    ThreadId&  getThreadId();

    InsnPtr_t   CreateInsn();

    void ConstructStage(const YAML::Node& StageConfig);

    void ConnectPipeline(const YAML::Node& StageConfig);

    void ConstructResource(const YAML::Node& ResourceConfig);

    void DistributeResource(const YAML::Node& ResourceConfig);

    ~Processor();

    void BuildTickSeq(const YAML::Node& StageConfig);

    void InsertStageToTickSeq(const YAML::Node& StageConfig,const std::string CurStageName);

    void Tick();

    void Advance();

    void Evaluate();

    void Reset(Addr_t boot_address);

    void FlushBackWard(InsnState_t StageTag);

    void FlushForward(InsnState_t StageTag);

};
    
} // namespace Emulator







#endif	