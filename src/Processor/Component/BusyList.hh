#ifndef __BUSYLIST_HH__
#define __BUSYLIST_HH__

#include "../../Common/Common.hh"
#include "../../Trace/Logging.hh"

#include <vector>


namespace Emulator
{

template <bool ZeroReg>
class BusyList : public Trace::TraceObject
{
private:

    struct BusyList_entry_t
    {
        bool    allocated;
        bool    forwarding;
        bool    done;
    };
    
    std::vector<BusyList_entry_t>   m_BusyList;

public:
    BusyList(std::string Name, const PhyRegId_t PhyRegCount)
    :   TraceObject(Name)
    {
        this->m_BusyList.resize(PhyRegCount);
    };
    ~BusyList(){};

    BusyList_entry_t& operator[](PhyRegId_t index){
        DASSERT((index < this->m_BusyList.size()), "index[{}] > {}",this->m_BusyList.size());
        return this->m_BusyList[index];
    };

    void Reset(bool RenameRegister = true){
        for(size_t i = 0 ; i < this->m_BusyList.size(); i++){
            if(!ZeroReg || i != 0){
                this->m_BusyList[i].allocated  = !RenameRegister;
                this->m_BusyList[i].forwarding = false;
                this->m_BusyList[i].done       = false  ; 
            }else{//0寄存器不能被分配
                this->m_BusyList[i].allocated  = true;
                this->m_BusyList[i].forwarding = false;
                this->m_BusyList[i].done       = true;
            }
        }
    }

};
    
} // namespace Emulator



#endif