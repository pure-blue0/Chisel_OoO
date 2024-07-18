#ifndef __DECODE_HH__
#define __DECODE_HH__ 

#include "BaseStage.hh"
#include "Pipe_data.hh"
#include "../Component/LoopQueue.hh"
#include "../../Common/Common.hh"

namespace Emulator
{

class Decode : public BaseStage
{
private:
    struct Pred_t
    {
        bool    taken_valid;
        bool    taken;
        bool    target_valid;
        Addr_t  target;
    };
    
    const uint16_t                      m_FetchByteWidth;

    const uint16_t                      m_iCacheAlignByte;

    LoopQueue<InsnPtr_t>        m_DecodeQueue;

public:
    std::vector<Pred_t>                 m_PredSync;

    Decode( Processor*          processor            ,
            const   uint64_t    DecodeQueueSize      
    );

    ~Decode();

    void Evaluate();

    void Reset();

    void ReleaseAction();

    void FlushAction();

    void ReceiveReq();

    void SendReq();

    void DecodeInsn(InsnPtr_t& insn);

    void Predecode(Emulator::DynInsn& insnEntry,InsnPkg_t& insnPkg,Redirect_message& redirect_message);

    void BranchRedirect(InsnPtr_t& insn,bool& needRedirect,Redirect_t& RedirectReq);
};

std::shared_ptr<BaseStage> Create_Decode_Instance(
        Processor*                       processor         ,
        const YAML::Node&                StageConfig    
);  

} // namespace Emulator


#endif	