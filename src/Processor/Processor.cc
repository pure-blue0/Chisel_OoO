#include "Processor.hh"

namespace Emulator
{

Processor::Processor(const YAML::Node& ProcessorConfig, BaseDRAM* DramPtr ,const ThreadId tid)
:   TraceObject(fmt::format("Processor[{}]",tid))
{
    this->m_DramPtr         = DramPtr;

    this->m_tid             = tid;

    this->m_XLEN            = ProcessorConfig["XLEN"].as<uint64_t>();

    this->m_doSpeculation   = ProcessorConfig["Speculation"].as<bool>();

    this->m_forwardResult   = ProcessorConfig["ForwardResult"].as<bool>();

    this->m_MemoryModel     = ProcessorConfig["MemoryModel"].as<std::string>();




    for(size_t i = 0; i < ProcessorConfig["SupportMode"].size(); i++){
        this->m_SupportMode.insert(ProcessorConfig["SupportMode"][i].as<std::string>());
    }
    for(size_t i = 0; i < ProcessorConfig["SupportISA"].size(); i++){
        this->m_SupportISA.insert(ProcessorConfig["SupportISA"][i].as<std::string>());
    }
    
    /* iCache */
    const YAML::Node& iCacheConfig = ProcessorConfig["iCache"];
    this->m_iCacheLatency          = iCacheConfig["iCacheLatency"].as<std::uint64_t>();
    this->m_iCacheAlignByte        = iCacheConfig["iCacheAlignByte"].as<std::uint64_t>();
    
    /* dCache */
    const YAML::Node& dCacheConfig = ProcessorConfig["dCache"];
    this->m_dCacheLatency          = dCacheConfig["dCacheLatency"].as<std::uint64_t>();
    this->m_dCacheAlignByte        = dCacheConfig["dCacheAlignByte"].as<std::uint64_t>();

    /* Rcu */
    const YAML::Node& RcuConfig             = ProcessorConfig["Rcu"];
    const bool        RenameRegister        = RcuConfig["RenameRegister"].as<bool>();
    const uint64_t    AllocWidth            = RcuConfig["AllocWidth"].as<uint64_t>();
    const uint64_t    DeallocWidth          = RcuConfig["DeallocWidth"].as<uint64_t>();
    
    const YAML::Node& RobConfig             = RcuConfig["ReorderBuffer"];
    const uint64_t    RobEntryCount         = RobConfig["EntryCount"].as<uint64_t>();

    const YAML::Node& IntRegFileConfig      = RcuConfig["PhysicalRegisterFile"]["Int"];
    const uint64_t    IntRegCount           = IntRegFileConfig["RegCount"].as<uint64_t>();

    const YAML::Node& RecoveryPolicyConfig  = RcuConfig["RecoveryPolicy"];
    bool              SupportRollBack       = RecoveryPolicyConfig["RollBack"].as<bool>();
    const uint64_t    SnapShotCount         = RecoveryPolicyConfig["SnapshotCount"].as<uint64_t>();

    /* Lsq Config */
    const YAML::Node& LsqConfig             = ProcessorConfig["Lsq"];
    const YAML::Node& LdqConfig             = LsqConfig["LoadQueue"];
    const uint64_t    LDQEntryCount         = LdqConfig["EntryCount"].as<uint64_t>();

    const YAML::Node& StqConfig             = LsqConfig["StoreQueue"];
    const uint64_t    STQEntryCount         = StqConfig["EntryCount"].as<uint64_t>();

    this->m_Rcu = std::make_shared<Rcu>(
        this,RenameRegister,RobEntryCount,
        IntRegCount,SupportRollBack,SnapShotCount,
        AllocWidth,DeallocWidth
    );

    this->m_Lsq = std::make_shared<Lsq>(this,m_dCacheAlignByte,LDQEntryCount,STQEntryCount);

    this->m_ExecContext = std::make_shared<RISCV::ExecContext>();

    /* Construct Resource */
    this->ConstructResource(ProcessorConfig["Resources"]);
    /* Distribute Resources */
    this->DistributeResource(ProcessorConfig["Resources"]);

    /* Construct Stage */
    this->ConstructStage(ProcessorConfig["Stages"]);
    /* Connect Stage */
    this->ConnectPipeline(ProcessorConfig["Stages"]);
    /* Build TickSequence */
    this->BuildTickSeq(ProcessorConfig["Stages"]);
}

Processor::~Processor(){}

BaseDRAM*
Processor::getDramPtr(){
    return this->m_DramPtr;
}

std::shared_ptr<Rcu> 
Processor::getRcuPtr(){
    return this->m_Rcu;
}
Fetch1* Processor::getFetch1Ptr(){
    return static_cast<Fetch1*>(this->m_StageMap["Fetch1"].get());
}

std::shared_ptr<Lsq>
Processor::getLsqPtr(){
    return this->m_Lsq;
}

ThreadId&
Processor::getThreadId(){
    return this->m_tid;
}

InsnPtr_t
Processor::CreateInsn(){
    auto Insn = std::make_shared<DynInsn>();
    Insn->State = InsnState_t::State_Fetch1;
    this->m_InsnPool.emplace_back(std::weak_ptr<DynInsn>(Insn));
    return Insn;
}
DecodeInsn_t
Processor::CreateDecodeInsn(){
    auto Insn = std::make_shared<DecodeQueue_entry>();
    this->m_DecodeInsnPool.emplace_back(std::weak_ptr<DecodeQueue_entry>(Insn));
    return Insn;
}

void
Processor::ConstructResource(const YAML::Node& ResourceConfig){
    /* Construct Register File Read Port */
    const YAML::Node& RfPortConfig = ResourceConfig["RegFileReadPort"];
    for(size_t i = 0 ; i < RfPortConfig.size(); i++){
        const uint64_t Latency = RfPortConfig[i]["Latency"].as<uint64_t>();
        this->m_RfReadPortVec.emplace_back(CreateRfReadPort(this,i,Latency));
    }

    /* Construct Write Back Port */
    const YAML::Node& WbPortConfig = ResourceConfig["WriteBackPort"];
    for(size_t i = 0 ; i < WbPortConfig.size(); i++){
        const uint64_t Latency = WbPortConfig[i]["Latency"].as<uint64_t>();
        this->m_WriteBackPortVec.emplace_back(CreateWriteBackPort(this,i,Latency));
    }


    /* Construct Function Unit */
    const YAML::Node& FuPoolConfig = ResourceConfig["FunctionUnitPool"];
    for(size_t i = 0; i < FuPoolConfig.size(); i++ ){
        const YAML::Node& FuConfig = FuPoolConfig[i];
        const std::string Name     = FuConfig["Name"].as<std::string>();
        const std::string FuncType = FuConfig["FuncType"].as<std::string>();
        const uint64_t    Latency  = FuConfig["Latency"].as<uint64_t>();
        const bool        Pipelined= FuConfig["Pipelined"].as<bool>();
        this->m_FuncUnitVec.emplace_back(FuncUnitFactory::CreateFuByName(this,FuncType,i,Name,Latency,Pipelined));
    }

    /* Construct Schedular */
    const YAML::Node& SchedularPoolConfig = ResourceConfig["Schedulers"];
    for(size_t i = 0; i < SchedularPoolConfig.size(); i++ ){
        const YAML::Node& SchedularConfig = SchedularPoolConfig[i];

        const std::string IssuePolicy     = SchedularConfig["IssuePolicy"].as<std::string>();
        const std::string WakeUpPolicy    = SchedularConfig["WakeUpPolicy"].as<std::string>();

        const std::string Name            = SchedularConfig["Name"].as<std::string>();
        const uint64_t    EntryCount      = SchedularConfig["EntryCount"].as<uint64_t>();
        const uint64_t    EnqueWidth      = SchedularConfig["EnqueWidth"].as<uint64_t>();
        const uint64_t    DequeWidth      = SchedularConfig["DequeWidth"].as<uint64_t>();
        this->m_SchedularVec.emplace_back(SchedularFactory::CreateSchedularByName(IssuePolicy,WakeUpPolicy,Name,i,EntryCount,EnqueWidth,DequeWidth,this->m_Rcu));
    }


}

void 
Processor::DistributeResource(const YAML::Node& ResourceConfig){
    /* Configure Forwarding Schedular for Write Back Port */
    const YAML::Node& WbPortConfig = ResourceConfig["WriteBackPort"];
    for(size_t i = 0 ; i < WbPortConfig.size(); i++){
        const YAML::Node& WakeUpSchedualersCfg = WbPortConfig[i]["WakeUpSchedualers"];
        for(size_t j = 0 ; j < WakeUpSchedualersCfg.size(); j++){
            const uint16_t SchedularId = WakeUpSchedualersCfg[j].as<uint16_t>();
            this->m_WriteBackPortVec[i]->AddForwardingSchedularId(SchedularId);
        }
    }

    /* Configure WriteBack Port for Function Unit */
    const YAML::Node& FuPoolConfig = ResourceConfig["FunctionUnitPool"];
    for(size_t i = 0; i < FuPoolConfig.size(); i++ ){
        const YAML::Node& FuConfig         = FuPoolConfig[i];
        const YAML::Node& WriteBackPortCfg = FuConfig["WriteBackPort"];
        for(size_t j = 0 ; j < WriteBackPortCfg.size(); j++){
            const uint16_t WbPortId = WriteBackPortCfg[j].as<uint16_t>();
            this->m_FuncUnitVec[i]->AddWbPort(this->m_WriteBackPortVec[WbPortId]->GetInPort());
        }
    }

    /* Configure Function Unit and RfReadPort for Schedular */
    const YAML::Node& SchedularPoolConfig = ResourceConfig["Schedulers"];
    for(size_t i = 0; i < SchedularPoolConfig.size(); i++ ){
        const YAML::Node& SchedularConfig  = SchedularPoolConfig[i];
        const YAML::Node& RFReadPortConfig = SchedularConfig["RFReadPort"];
        for(size_t j = 0; j < RFReadPortConfig.size(); j++){
            const uint16_t RfPortId = RFReadPortConfig[j].as<uint16_t>();
            this->m_SchedularVec[i]->AddRFReadPort(this->m_RfReadPortVec[RfPortId]->GetInPort());
        }
        const YAML::Node& FuncUnitPoolConfig = SchedularConfig["FunctionUnitPool"];
        for(size_t j = 0; j < FuncUnitPoolConfig.size(); j++){
            const uint16_t FuId = FuncUnitPoolConfig[j].as<uint16_t>();
            this->m_SchedularVec[i]->AddFuncUnit(this->m_FuncUnitVec[FuId]);
        }
    }
    /* Configure SubSchedular */
    for(size_t i = 0; i < SchedularPoolConfig.size(); i++ ){
        const YAML::Node& SchedularConfig  = SchedularPoolConfig[i];
        const YAML::Node& SubSchedularConfig = SchedularConfig["SubScheduler"];
        for(size_t j = 0; j < SubSchedularConfig.size(); j++){
            const uint16_t ScheId = SubSchedularConfig[j].as<uint16_t>();
            this->m_SchedularVec[i]->AddSubSchedular(this->m_SchedularVec[ScheId]);//不止会把subscheduler与scheduler连接，还会把自己支持的funct也根据subscheduler支持的funct进行设置
        }
    }
}

void 
Processor::ConstructStage(const YAML::Node& StageConfig){
    for(YAML::const_iterator it = StageConfig.begin(); it != StageConfig.end(); it++){
        std::string       Name      = it->first.as<std::string>();
        const YAML::Node& StageInfo = it->second["StageInfo"];
        this->m_StageMap[Name]      = StageFactory::CreateStageByName(Name,this,StageInfo);
    }
}

void 
Processor::ConnectPipeline(const YAML::Node& StageConfig){
    for(YAML::const_iterator it = StageConfig.begin(); it != StageConfig.end(); it++){
        const std::string CurStageName  = it->first.as<std::string>();
        const std::string NextStageName = it->second["NextStage"].as<std::string>();

        if(NextStageName != "None"){
            const std::string ForwardLatchName        = fmt::format("{}To{}Latch",CurStageName,NextStageName);
            const uint64_t    Latency                 = it->second["Latency"].as<uint64_t>();
            const uint64_t    ProcessInsnCount        = StageConfig[NextStageName]["MaxProcessInsnCount"].as<int64_t>();
            const bool        MustTakenAllInsn        = it->second["MustTakenAllInsn"].as<bool>();
            const std::string AckLatchName            = fmt::format("{}To{}AckLatch",NextStageName,CurStageName);
            TimeBuffer<InsnPkg_t>* InterStageLatchPtr = new TimeBuffer<InsnPkg_t>(ForwardLatchName,Latency);
            TimeBuffer<StageAck_t>* AckLatchPtr       = new TimeBuffer<StageAck_t>(AckLatchName,0);
            this->m_StageMap[CurStageName]->m_MustTakenAllInsn = MustTakenAllInsn;
            this->m_StageMap[CurStageName]->ConnectStageOutPort(InterStageLatchPtr->InPort,ProcessInsnCount);
            this->m_StageMap[CurStageName]->ConnectStageAckInPort(AckLatchPtr->OutPort);

            this->m_StageMap[NextStageName]->ConnectStageInPort(InterStageLatchPtr->OutPort,ProcessInsnCount);
            this->m_StageMap[NextStageName]->ConnectStageAckOutPort(AckLatchPtr->InPort);

            this->m_ForwardConnectionLatch.emplace_back(std::shared_ptr<TimeBuffer<InsnPkg_t>>(InterStageLatchPtr));  
            this->m_BackwardConnectionLatch.emplace_back(std::shared_ptr<TimeBuffer<StageAck_t>>(AckLatchPtr));
        }

        const int RedirectLatency = it->second["RedirectLatency"].as<int>();
        if(!(RedirectLatency == -1)){
            const std::string RedirectLatchName        = fmt::format("{}ToFetch1Latch",CurStageName);
            TimeBuffer<Redirect_t>* RedirectLatchPtr   = new TimeBuffer<Redirect_t>(RedirectLatchName,RedirectLatency);
            this->m_StageMap[CurStageName]->ConnectRedirectPort(RedirectLatchPtr->InPort);
            static_cast<Fetch1*>(this->m_StageMap["Fetch1"].get())->AddRedirectPort(RedirectLatchPtr->OutPort);
            this->m_RedirectConnectionLatch.emplace_back(std::shared_ptr<TimeBuffer<Redirect_t>>(RedirectLatchPtr)); 
        }
    }
}

void
Processor::BuildTickSeq(const YAML::Node& StageConfig){
    this->InsertStageToTickSeq(StageConfig,"Fetch1");//fetch1->fetch2->decode->dispatch->iew
    this->InsertStageToTickSeq(StageConfig,"Commit");//only commit
    if(!(this->m_StageMap.size() == this->m_StageTickSeq.size())){//not insert all stage
        SPDLOG_ERROR("BuildTickSeq Failed");
        exit(-1);
    }
}

void
Processor::InsertStageToTickSeq(const YAML::Node& StageConfig,const std::string CurStageName){
    const YAML::Node& CurStageConfig    = StageConfig[CurStageName];
    const uint64_t    CurStageLatency   = CurStageConfig["Latency"].as<uint64_t>();
    const InsnState_t CurStageTag       = StageFactory::LookupEnumByName(CurStageName); 
    const std::pair<InsnState_t,std::shared_ptr<BaseStage>>insertContext(CurStageTag,this->m_StageMap[CurStageName]);

    const std::string NextStageName     = CurStageConfig["NextStage"].as<std::string>();
    if(NextStageName == "None" || NextStageName == CurStageName){//when stage is iew or commit enter
        this->m_StageTickSeq.emplace_back(insertContext);
    }else{
        const InsnState_t    NextStageTag      = StageFactory::LookupEnumByName(NextStageName);
        bool  insertSuccess = false;
        while(!insertSuccess){
            for(auto it = this->m_StageTickSeq.begin(); it != this->m_StageTickSeq.end(); it++){
                if(it->first == NextStageTag){
                    insertSuccess = true;
                    if(CurStageLatency == 0){
                        this->m_StageTickSeq.insert(it-1,insertContext);
                    }else{
                        this->m_StageTickSeq.insert(it+1,insertContext);
                    }
                }
            }
            if(!insertSuccess){
                this->InsertStageToTickSeq(StageConfig,NextStageName);
            }
        }
    }
}

void
Processor::Advance(){
    // Must Tick Stage At First to do flush/release Action
    for(auto it : this->m_StageTickSeq){
        it.second->Advance();
    } 
    /* Can be executed Out of Order */
    for(auto it : this->m_ForwardConnectionLatch){
        it->advance();
    }
    for(auto it : this->m_BackwardConnectionLatch){
        it->advance();
    }
    for(auto it : this->m_RedirectConnectionLatch){
        it->advance();
    }
    for(auto it : this->m_SchedularVec){
        it->Advance();
    }
    for(auto it : this->m_FuncUnitVec){
        it->Advance();
    }
    for(auto it : this->m_WriteBackPortVec){
        it->Advance();
    }
    for(auto it : this->m_RfReadPortVec){
        it->Advance();
    }
}

void
Processor::Evaluate(){
    
    for(auto it : this->m_StageTickSeq){
        it.second->Evaluate();
    }
    this->m_Rcu->Evaulate();
    this->m_Lsq->Evaulate();
    (static_cast<Decode*>(this->m_StageMap["Decode"].get()))->decodeQueue_flush=false;
    (static_cast<Fetch1*>(this->m_StageMap["Fetch1"].get()))->fetchQueue_flush=false;

    for(int i=0;i<4;i++){
        this->m_Rcu->ROB_WB_EN_Group[i]=false;
        this->m_Rcu->ROB_Entry_WEN_GROUP[i]=false;
    }
    for(int i=0;i<4;i++){
        this->m_Lsq->Load_WEN_Group[i]=false;
        this->m_Lsq->Store_WEN_Group[i]=false;
        this->m_Lsq->LSQ_Style_Group[i]=0;

        this->m_Lsq->KillLoadEntry_flag[i]=false;
        this->m_Lsq->KillStoreEntry_flag[i]=false;
    }
    for(int i=0;i<4;i++){
        this->m_Rcu->RN_Release_EN[i]=false;
    }
  
    this->m_Rcu->lsq_count=0;
    this->m_Rcu->pop_num=0;
    
    this->m_Lsq->LSU_Style_Group[0]=0;
    this->m_Lsq->LSU_Style_Group[1]=0;
}

void 
Processor::Tick(){
    for(auto insn : this->m_InsnPool){
        if(insn.expired()){
            this->m_InsnPool.pop_front();
        }
    }
    this->m_ExecContext->CycleInc();
    this->Evaluate();
    this->Advance();
}

void 
Processor::Reset(Addr_t boot_address){
    for(auto it : this->m_StageTickSeq){
        it.second->Reset();
    }
    for(auto it : this->m_ForwardConnectionLatch){
        it->reset();
    }
    for(auto it : this->m_BackwardConnectionLatch){
        it->reset();
    }
    for(auto it : this->m_RedirectConnectionLatch){
        it->reset();
    }
    for(auto it : this->m_SchedularVec){
        it->Reset();
    }
    for(auto it : this->m_FuncUnitVec){
        it->Reset();
    }
    for(auto it : this->m_WriteBackPortVec){
        it->Reset();
    }
    for(auto it : this->m_RfReadPortVec){
        it->Reset();
    }
    static_cast<Fetch1*>(m_StageMap["Fetch1"].get())->InitBootPc(boot_address);
    this->m_Rcu->Reset();
    this->m_Lsq->Reset();
    this->m_ExecContext->Reset(this->m_tid,boot_address);


}


void 
Processor::FlushBackWard(InsnState_t StageTag){
    
    for(auto it : this->m_StageTickSeq){
        if(it.first < StageTag){
            it.second->Flush();
        }
    }
}

void 
Processor::FlushForward(InsnState_t StageTag){
    for(auto it : this->m_StageTickSeq){
        if(it.first > StageTag){
            it.second->Flush();
        }
    }
}

} // namespace Emulator
