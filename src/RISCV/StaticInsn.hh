#ifndef __STATICINSN_HH__
#define __STATICINSN_HH__ 

#include "../Common/Common.hh"

namespace RISCV
{
    
class StaticInsn{

public:

    StaticInsn(uint32_t insn){
        this->m_insn = insn;
    };

    ~StaticInsn(){};

    uint32_t operator()(const uint8_t pos){//取出指定位的数据
        return (this->m_insn & ((uint64_t)1 << pos)) >> pos;
    };

    uint32_t operator()(const uint8_t upper, const uint8_t lower){//取出对应范围的数据
        return ( ( this->m_insn & (((uint64_t)1 << (upper+1)) - 1) ) & ~(((uint64_t)1 << lower) - 1) ) >> lower;
    };

    uint8_t rs1(){
        return this->operator()(19,15);
    };
    uint8_t rs2(){
        return this->operator()(24,20);
    };
    uint8_t rs3(){
        return this->operator()(31,27);
    };
    uint8_t rd(){
        return this->operator()(11,7);
    };
    uint32_t opcode(){
        return this->operator()(6,0);
    };
    uint32_t func3(){
        return this->operator()(14,12);
    };
    uint32_t func7(){
        return this->operator()(31,25);
    };
    int32_t iimm(){
        return (int32_t)this->m_insn >> 20;
    };

    int32_t simm(){
        return (((int32_t)this->m_insn >> 31) & ~((uint32_t)((1 << 11)-1))) +
                (this->operator()(30,25) << 5) + 
                (this->operator()(11,8) << 1) + 
                (this->operator()(7));
    };
    int32_t sbimm(){
        return (((int32_t)this->m_insn >> 31) & ~((uint32_t)((1 << 12)-1))) +
                (this->operator()(7) << 11) +
                (this->operator()(30,25) << 5) + 
                (this->operator()(11,8) << 1);
    };
    int32_t uimm(){
        return (int32_t)this->m_insn & ~((uint32_t)((1 << 12) - 1));
    };
    int32_t ujimm(){
        return (((int32_t)this->m_insn >> 31) & ~((uint32_t)((1 << 20)-1))) + 
                (this->operator()(19,12) << 12) +  
                (this->operator()(20)    << 11) +   
                (this->operator()(30,25) << 5) +   
                (this->operator()(24,21) << 1);
    };
    int32_t shamt(){
        return this->operator()(25,20);
    };
    int32_t csr(){
        return this->operator()(31,20);
    };
	int32_t rvc_j_imm() {
		return (((int32_t)((this->operator()(12) << 11) +
				(this->operator()(11) << 4) + 
				(this->operator()(10) << 9) +
				(this->operator()(9) << 8) +
				(this->operator()(8) << 10) +
				(this->operator()(7) << 6) +
				(this->operator()(6) << 7) +
				(this->operator()(5) << 3) +			
				(this->operator()(4) << 2) +				
				(this->operator()(3) << 1) +				
				(this->operator()(2) << 5)) << 20) >> 20);	
	};
	int32_t rvc_b_imm() {
		return (((int32_t)((this->operator()(12) << 8) +
				(this->operator()(11) << 4) + 
				(this->operator()(10) << 3) +
				(this->operator()(6) << 7) +
				(this->operator()(5) << 6) +
				(this->operator()(4) << 2) +
				(this->operator()(3) << 1) +
				(this->operator()(2) << 5)) << 23) >> 23);	
	};

protected:
    uint32_t m_insn;
};

} // namespace RISCV





#endif	