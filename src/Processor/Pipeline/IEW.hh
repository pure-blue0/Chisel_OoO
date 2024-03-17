#ifndef __IEW_HH__
#define __IEW_HH__

#include "BaseStage.hh"
#include "../Component/Schedular/BaseSchedular.hh"
#include "../Component/FuncUnit/BaseFuncUnit.hh"
#include "../Component/WriteBackPort.hh"
#include "../Component/RfReadPort.hh"

namespace Emulator
{

class Processor;
class IEW : public BaseStage
{
private:

    std::vector<std::shared_ptr<BaseFuncUnit>>   m_ExceuteStageVec;

    std::vector<std::shared_ptr<BaseSchedular>>  m_IssueStageVec;

    std::vector<std::shared_ptr<RfReadPort>>     m_ReadOperandStageVec;

    std::vector<std::shared_ptr<WriteBackPort>>  m_WriteBackStageVec;

public:
    IEW(Processor* processor);

    ~IEW();

    void Reset();

    void Flush();
    
    void Evaluate();

    void Issue();

    void ReadOperand();

    void Execute();

    void Forwarding();

    void WriteBack();

    void AddFuncUnitToExecStage(std::shared_ptr<BaseFuncUnit> fu);

    void AddSchedularToIssueStage(std::shared_ptr<BaseSchedular> schedular);

    void AddRfReadPortToReadOperandStage(std::shared_ptr<RfReadPort> rfRdPort);

    void AddWbPortToWriteBackStage(std::shared_ptr<WriteBackPort> wbPort);

};
    
std::shared_ptr<BaseStage> Create_IEW_Instance(
        Processor*                       processor         ,
        const YAML::Node&                StageConfig    
);

} // namespace Emulator



#endif