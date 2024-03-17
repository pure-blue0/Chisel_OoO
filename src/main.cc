#include <iostream>
#include "cmdline.h"
#include "Simulator.hh"
#include "ELFParser/ElfParser.hh"

using namespace std;

std::vector<std::string> split(std::string str, std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str += pattern;
    int size = str.size();
    for (int i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}


int main(int argc, char *argv[])
{
    /* RunTime args Parser */
    cmdline::parser cmdParser;
    cmdParser.add<string>  ("config"     , 'c' , "Simulator Config Path" ,true);
    cmdParser.add<string>  ("elf"        , 'e' , "elf Path"              ,true);
    cmdParser.add<string>  ("log"        , 'l' , "Log Path"              ,false);
    cmdParser.add<string>  ("debugFlag"  , 'd' , "Open Debug Log"        ,false);
    cmdParser.add<uint64_t>("MaxCycle"   , 'm' , "Max Tick Cycle"        ,false, -1);
    cmdParser.add          ("debugHelp"  , 'h' , "Print debug Flags description");
    cmdParser.parse_check(argc,argv);

    if(cmdParser.exist("debugHelp")){
        cout << "[---------- Debug flags descriptions ----------]" << endl;
        for( auto pair : Emulator::debugFlags){
            cout << fmt::format("[ {:<17} {:^12} {:>17} ]",pair.first,"|",pair.second) << endl;
        }
        cout << "\n";
        cout << "[---------- Debug Object descriptions ----------]" << endl;
        for( auto pair : Emulator::registedObject){
            cout << fmt::format("[ {:<17} {:^12} {:>17} ]",pair.first,"|",pair.second) << endl;
        }
        exit(0);
    }

    /* Trace Log Config */
    const bool    console  = !cmdParser.exist("log");
    const bool    debug    = cmdParser.exist("debugFlag");
    const string  logPath  = cmdParser.get<string>("log");
    Emulator::Trace::initLogger(debug,console,logPath);

    /* Init Debug Flags */
    if(debug){
        const vector<string> flags = split(cmdParser.get<string>("debugFlag"),",");
        bool                 openall = false;
        for(auto flag : flags){
            if(flag == string("ALL")){
                openall = true;
            }
        }
        if(openall){
            for(auto flag : Emulator::debugFlags){
                Emulator::setFlagEnable(flag.first);
            }
            for(auto object : Emulator::registedObject){
                Emulator::setObjectEnable(object.first);
            }
        } else {
            for(auto flag : flags){
                if(Emulator::debugFlags.count(flag)){
                    Emulator::setFlagEnable(flag);
                }else if(Emulator::registedObject.count(flag)){
                    Emulator::setObjectEnable(flag);
                }else{
                    SPDLOG_ERROR("Known Flag : {}",flag);
                    exit(-1);
                }
            }
        }
    }

    /* Yaml Config Parser */
    const YAML::Node config = YAML::LoadFile(cmdParser.get<string>("config"));

    Emulator::Simulator Simulator(config["Simulator"]);

    /* Elf Parser */
    ELFParser elfloader(cmdParser.get<string>("elf"));

    Emulator::BaseDRAM* Dram = Simulator.GetDRAM();

    elfloader.Load(Dram);

    spdlog::info("Load Elf({}) Successfully",cmdParser.get<string>("elf"));

    /* Reset Simulator */
    Simulator.Reset(elfloader.get_entry());

    spdlog::info("RESET Simulator Successfully");

    uint64_t tohost;
    uint64_t exitCode = 0;
    for (size_t i = 0; i < cmdParser.get<uint64_t>("MaxCycle"); i++)
    {
        Simulator.Tick();
        tohost = Dram->readDouble(elfloader.get_tohost());
        if(tohost){
            if(tohost & 0b1){ //Exit Event
                exitCode = tohost >> 0b1;
                if(exitCode){
                    spdlog::error("[FAIL] exit code : [{:#x}]",exitCode);
                }else{
                    spdlog::info("[PASS] exit code : [{:#x}]",exitCode);
                }
                break;
            }else{
                std::stringstream outString;
                if(Dram->checkRange(tohost)){
                    bool        printEvent = Dram->readDouble(tohost) == 64;
                    if(printEvent){
                        uint64_t    stringAddr = Dram->readDouble(tohost+16);
                        uint64_t    stringLen = Dram->readDouble(tohost+24);
                        for(size_t lo = 0 ; lo < stringLen; lo++){
                            outString << Dram->readByte(stringAddr+lo);
                        }
                        Dram->writeDouble(elfloader.get_fromhost(),1);
                        Dram->writeDouble(elfloader.get_tohost(),0);
                        std::cout <<  outString.str() << std::endl;
                    }
                }
            }
        }
    }
    return exitCode;
}
