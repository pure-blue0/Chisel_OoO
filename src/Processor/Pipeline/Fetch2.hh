#ifndef __FETCH2_HH__
#define __FETCH2_HH__ 


#include "BaseStage.hh"
#include "Pipe_data.hh"
#include "../Component/LoopQueue.hh"

#include "../../Common/Common.hh"

namespace Emulator
{
    
class Fetch2 : public BaseStage
{
private:

    LoopQueue<InsnPtr_t>    m_InsnBuffer;

public:
    Fetch2( Processor*          processor            ,
            const   uint64_t    InstrBufferSize      
    );

    ~Fetch2();
    
    void Evaluate();     
    
    void Reset();       
    
    void ReleaseAction();

    void FlushAction();

    void ReceiveReq();

    void SendReq();
};

std::shared_ptr<BaseStage> Create_Fetch2_Instance(
        Processor*                       processor         ,
        const YAML::Node&                StageConfig    
);  

} // namespace Emulator


#endif	