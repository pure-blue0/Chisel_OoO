#include "ElfParser.hh"
#include "spdlog/spdlog.h"
ELFParser::ELFParser(const std::string elfPath){
    if(!reader.load(elfPath)){
        SPDLOG_ERROR("Can't find or process ELF file");
        exit(-1);
    }
}
ELFParser::~ELFParser(){};

section* 
ELFParser::get_symtab(){
    for ( int i = 0; i < reader.sections.size(); ++i ) {
        section* psec = reader.sections[i];
        if(psec->get_name() == ".symtab"){
            return psec;
        }
    }
    SPDLOG_ERROR("Can't get symble table");
    exit(-1);
}

uint64_t 
ELFParser::read_symtab_by_name(const std::string ref_name){
    const symbol_section_accessor symbols( reader, get_symtab() );
    for ( unsigned int j = 0; j < symbols.get_symbols_num(); ++j ) {
        std::string name;
        Elf64_Addr value;
        Elf_Xword size;
        unsigned char bind;
        unsigned char type;
        Elf_Half section_index;
        unsigned char other;
        symbols.get_symbol( j, name, value, size, bind,type, section_index, other ); 
        if(name == ref_name){
            return value;
        }
    }
    SPDLOG_ERROR("Can't get {} in symble table",ref_name.c_str());
    exit(-1);
}

uint64_t 
ELFParser::get_entry(){
    return read_symtab_by_name("_start");
}

uint64_t 
ELFParser::get_size(){
    return read_symtab_by_name("_end") - read_symtab_by_name("_start");
}

uint64_t 
ELFParser::get_tohost(){
    return read_symtab_by_name("tohost");
}

uint64_t 
ELFParser::get_fromhost(){
    return read_symtab_by_name("fromhost");
}

void 
ELFParser::Load(char* mem){
    for( size_t i = 0; i < reader.segments.size(); i++){
        const segment* pseg = reader.segments[i];
        const size_t   size = pseg->get_file_size();
        const uint64_t addr = pseg->get_physical_address() - get_entry();
        const char*    data = pseg->get_data();
        if(data != NULL){
            memcpy(mem+addr,data,size);
        }
    }
}

void 
ELFParser::Load(Emulator::BaseDRAM* dram){
    if(dram->size() < this->get_size()){
        SPDLOG_ERROR("DRAM Size : {}  Less Than Elf Size : {}", dram->size(), this->get_size());
        exit(-1);
    }
    for( size_t i = 0; i < reader.segments.size(); i++){
        const segment* pseg = reader.segments[i];
        const size_t   size = pseg->get_file_size();
        const uint64_t addr = pseg->get_physical_address();
        const char*    data = pseg->get_data();
        if(data){
            dram->write(addr,data,size);
        }
    }
}

