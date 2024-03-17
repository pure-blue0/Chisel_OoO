#ifndef __COMMIT_HH__
#define __COMMIT_HH__

#include "BaseStage.hh"

namespace Emulator
{
    
class Processor;
class Commit : public BaseStage
{
private:
    /* data */
public:
    
    Commit(Processor* processor);
    
    ~Commit();

    void Reset();

    void Flush();

    void SendCommitReq();

    void CommitInsnPkg();

    void Evaluate();
};


std::shared_ptr<BaseStage> Create_Commit_Instance(
        Processor*                       processor         ,
        const YAML::Node&                StageConfig    
);  






} // namespace Emulator


#endif