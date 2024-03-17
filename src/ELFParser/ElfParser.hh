#ifndef __ELFPARSER_HH__
#define __ELFPARSER_HH__

#include "elfio/elfio.hpp"
#include "../Memory/BaseDRAM.hh"
#include <string>

using namespace ELFIO;

class ELFParser
{
private:
     ELFIO::elfio reader;
public:

    ELFParser(const std::string elfPath);

    ~ELFParser();

    section* get_symtab();

    uint64_t read_symtab_by_name(const std::string ref_name);
    
    uint64_t get_entry();

    uint64_t get_size();

    uint64_t get_tohost();

    uint64_t get_fromhost();

    void Load(char* mem);

    void Load(Emulator::BaseDRAM* dram);
};


#endif