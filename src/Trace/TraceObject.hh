#ifndef __TRACEOBJECT_HH__
#define __TRACEOBJECT_HH__


#include <string>

namespace Emulator
{
namespace Trace
{
    
class TraceObject
{
private:

    const std::string m_Name;

public:

    TraceObject(const std::string Name)
    :   m_Name(Name)
    {};

    ~TraceObject() {};

    const std::string Name(){
        return m_Name;
    };

};


} // namespace Trace

} // namespace Emulator




#endif