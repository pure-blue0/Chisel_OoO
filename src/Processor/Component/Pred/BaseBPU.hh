#ifndef __BASEBPU_HH__
#define __BASEBPU_HH__

#include <stdint.h>
#include <vector>
#include "../../../Common/Common.hh"
#include "../../../Trace/TraceObject.hh"

namespace Emulator
{

struct baseMeta_t
{
    uint16_t index;
    uint16_t Brmask;
};

struct basePred_t
{
    bool valid;
    bool taken;
};

struct baseRes_t{
    uint16_t                index;
    std::vector<basePred_t> pred;
};

class BaseBPU : public Trace::TraceObject
{
private:

public:

    BaseBPU(/* args */);

    ~BaseBPU();

    virtual baseRes_t              Predict(Addr_t Pc);

    virtual void                   Update(baseMeta_t meta, bool taken, uint16_t offset);

    virtual void                   Flush();

    virtual void                   Reset();

};
    
} // namespace Emulator




#endif