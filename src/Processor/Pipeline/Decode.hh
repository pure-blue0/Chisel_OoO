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

    LoopQueue<InsnPtr_t>        m_DecodeQueue;

public:
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
};

std::shared_ptr<BaseStage> Create_Decode_Instance(
        Processor*                       processor         ,
        const YAML::Node&                StageConfig    
);  

} // namespace Emulator


#endif	