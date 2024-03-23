#ifndef __DISPATCH_HH__
#define __DISPATCH_HH__

#include "BaseStage.hh"
#include "../Component/Schedular/BaseSchedular.hh"

namespace Emulator
{
class Processor;
class Dispatch : public BaseStage
{
private:

    struct Disp_Schedular_t
    {
        std::shared_ptr<BaseSchedular> scheduler;
        uint64_t AvailPort;
    };
    

    std::vector<Disp_Schedular_t> m_SchedularVec;

public:
    Dispatch(
        Processor* processor
    );

    ~Dispatch();

    void AddSchedular(std::shared_ptr<BaseSchedular> Schedular);

    void Reset();

    void Evaluate();

    void TryDispatch(InsnPkg_t& insnPkg, uint64_t& SuccessCount, bool CheckControlFlow = false);

    void DispatchInsn(InsnPkg_t& insnPkg, uint64_t DispatchCount);
};

std::shared_ptr<BaseStage> Create_Dispatch_Instance(
        Processor*                       processor         ,
        const YAML::Node&                StageConfig    
);  

} // namespace Emulator


#endif