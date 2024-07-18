#ifndef __FETCH1_HH__
#define __FETCH1_HH__ 

#include "BaseStage.hh"
#include "Pipe_data.hh"

#include "../Component/iCachePort.hh"
#include "../Component/LoopQueue.hh"

#include "../../Common/Common.hh"
#include "../../Memory/BaseDRAM.hh"

namespace Emulator
{
class Simulator;
class Processor;
class Fetch1 : public BaseStage
{
    friend class commit;
private:
    enum   State_t{
        Idle, HandleExcp, WaitForResume
    };
    struct Pred_t
    {
        bool    taken_valid;
        bool    taken;
        bool    target_valid;
        Addr_t  target;
    };
    struct InflighQueueEntry_t
    {
        bool                        Busy;
        bool                        Killed;
        Addr_t                      Address;
        std::vector<char>           InsnByte;
        Exception_t                 Excp;
        InflighQueueEntry_t(){};//结构体的构造函数
        InflighQueueEntry_t(uint64_t ByteWidth){//结构体的构造函数（含参）
            this->InsnByte.resize(ByteWidth);
        };
    };

private:
    /* Param */
    const uint16_t                      m_FetchByteWidth;

    const uint16_t                      m_iCacheAlignByte;

    bool                                m_MisalignValid;

    uint16_t                            m_MisalignHalf;

    /* Var */
    State_t                             m_State;

    uint64_t                            m_ExcpTag;

    TimeBuffer<Addr_t>                  m_PcRegister;

    std::vector<Pred_t>                 m_PredSync;

    /* Ptr in this Module */
    iCachePort                          m_iCachePort;

    LoopQueue<InflighQueueEntry_t>      m_InflightQueue;

    /* Ptr Out of this Module */
    std::vector<std::shared_ptr<TimeBuffer<Redirect_t>::Port>> m_RedirectPortVec;
    

public:

    TimeBuffer<Redirect_message>* Decode_Redirect_Reg = new TimeBuffer<Redirect_message>("Decode_Redirect_Reg", 1);
    TimeBuffer<Redirect_message>* IEW_Redirect_Reg = new TimeBuffer<Redirect_message>("IEW_Redirect_Reg", 1);
    TimeBuffer<Redirect_message>* Commit_Redirect_Reg = new TimeBuffer<Redirect_message>("Commit_Redirect_Reg", 1);

    Fetch1( 
        Processor*                       processor              ,
        const uint64_t                   iCacheLatency          ,
        const uint64_t                   iCacheAlignByte        ,
        const uint64_t                   FetchByteWidth         ,
        const uint64_t                   InflightQueueSize 
    );

    ~Fetch1();

    void Evaluate();

    void Advance();

    void InitBootPc(Addr_t boot_address);

    void Reset();
    bool Send_Data_to_Fetch();

    void FlushAction();

    void KillNewer(uint64_t InflightQueueTag);

    void GenNextFetchAddress(bool& SendSuccess);

    void SendFetchReq(bool& SendSuccess,MemReq_t& fetchReq,InflighQueueEntry_t& NewEntry);

    void ReceiveReq(MemResp_t mem_resp);

    void Predecode(InflighQueueEntry_t& frontEntry ,InsnPkg_t& insnPkg);

    void BranchRedirect(InsnPtr_t& insn, Pred_t& Pred, bool& needRedirect, Redirect_t& RedirectReq);

    void Predict(Addr_t& Pc, std::vector<Pred_t>& insnPred);

    void SendFetchData(bool& pop_flag);

    void AddRedirectPort(std::shared_ptr<TimeBuffer<Redirect_t>::Port> RedirectPort);

    void InflightQueue_Evaluate(bool SendSuccess,bool resp_valid,uint16_t TransId,InflighQueueEntry_t NewEntry,MemResp_t resp);

};

std::shared_ptr<BaseStage> Create_Fetch1_Instance(
        Processor*                       processor         ,
        const YAML::Node&                StageConfig    
);  
} // namespace Emulator





#endif	