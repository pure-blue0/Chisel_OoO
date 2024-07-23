#ifndef __DECODE_HH__
#define __DECODE_HH__ 

#include "BaseStage.hh"
#include "Pipe_data.hh"
#include "../Component/LoopQueue.hh"
#include "../../Common/Common.hh"
#define DecodeQueue_Size 60
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
    
    //decode queue flag.

    

public:
    std::vector<Pred_t>                 m_PredSync;
    bool decodeQueue_flush;
    Decode( Processor*          processor            ,
            const   uint64_t    DecodeQueueSize      
    );

    ~Decode();

    void Evaluate();

    void Reset();

    void ReleaseAction();

    void FlushAction();

    void SendReq();

    void DecodeQueue_Evaluate(bool reset_n,bool decodeQueue_flush, uint8_t pop_count,bool WEN1,DecodeInsn_t insn1_WriteIn,bool WEN2,DecodeInsn_t insn2_WriteIn,bool isStalled,uint8_t& r_avail_count);

    void DecodeInsn(DecodeInsn_t& insn);

    void Predecode(Emulator::DynInsn& insnEntry,DecodeInsn_t& insn1,DecodeInsn_t& insn2,Redirect_message& redirect_message,bool& flush_flag);

    void BranchRedirect(DecodeInsn_t& insn,Redirect_message& redirect_message);
};

std::shared_ptr<BaseStage> Create_Decode_Instance(
        Processor*                       processor         ,
        const YAML::Node&                StageConfig    
);  

} // namespace Emulator


#endif	