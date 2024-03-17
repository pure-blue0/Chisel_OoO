#ifndef __FREELIST_HH__
#define __FREELIST_HH__

#include "../../Common/Common.hh"
#include "../../Trace/Logging.hh"

#include <deque>

namespace Emulator
{

template <bool ZeroReg>
class FreeList : public Trace::TraceObject
{
private:

    const PhyRegId_t       m_PhyRegCount;

    std::deque<PhyRegId_t> m_Freelist;

public:
    FreeList(const std::string Name, const PhyRegId_t phyRegCount)
    :   TraceObject(Name), m_PhyRegCount(ZeroReg ? (phyRegCount - 1) : (phyRegCount))
    {
    };

    ~FreeList(){};

    void Reset(){
        for(PhyRegId_t i = 0 ; i < this->m_PhyRegCount ; i++){
            if(!ZeroReg | (i != 0)){
                this->m_Freelist.push_back(i);
            }
        }
    };

    bool empty(){
        return this->m_Freelist.empty();
    };

    bool full(){
        return this->m_Freelist.size() == m_PhyRegCount;
    };

    PhyRegId_t front(){
        return this->m_Freelist.front();
    };
    
    PhyRegId_t pop(){
        DASSERT(!this->m_Freelist.empty(),"Pop When empty!")
        PhyRegId_t t = this->m_Freelist.front();
        this->m_Freelist.pop_front();
        return t;
    };

    void push(PhyRegId_t t){
        #ifdef TRACE_ON
        for(PhyRegId_t i = 0 ; i < this->m_Freelist.size(); i++){
            DASSERT((this->m_Freelist[i]!=t),"Push[{}] With the same regfile in queue",t);
        }
        #endif
        this->m_Freelist.push_back(t);
    };

    PhyRegId_t getAvailEntryCount(){
        return this->m_Freelist.size();
    }

};

} // namespace Emulator


#endif