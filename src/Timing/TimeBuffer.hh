#ifndef __TIMEBUFFER_HH__
#define __TIMEBUFFER_HH__ 

#include "Tick.hh"
#include "../Trace/TraceObject.hh"
#include <queue>
#include <memory>
#include <iostream>
namespace Emulator
{

template <typename T>
class TimeBuffer : public Trace::TraceObject
{
friend class Port;
public:
    struct timebuf_entry_t
    {
        T           data;
        uint64_t    DestTick;
    };
    typedef std::queue<timebuf_entry_t> timebuf_t;

    class Port
    {
    friend class TimeBuffer;
    public:
        Port(
            std::shared_ptr<bool>&  stallPtr,
            std::shared_ptr<bool>&  flushPtr
        ) : m_stall(stallPtr),m_flush(flushPtr)
        {
            this->valid   = false;
        };
        ~Port(){};
        void set(T data){
            this->valid = true;
            this->data  = data;
        };
        void kill(){
            this->valid = false;
        };
        void flush(){
            *this->m_flush = true;
        };
        void stall(){
            *this->m_stall = true;
        };
        bool isStalled(){
            return *this->m_stall;
        };
    public:
        bool        valid;
        T           data;
    private:
        std::shared_ptr<bool>  m_stall;
        std::shared_ptr<bool>  m_flush;
    };

    TimeBuffer(std::string name, uint64_t latency) :
        TraceObject(name),
        m_latency(latency)
    {
        this->m_flush     = std::make_shared<bool>(false);
        this->m_stall     = std::make_shared<bool>(false);
        if(latency == 0){
            this->InPort  = std::make_shared<Port>(this->m_stall,this->m_flush);
            this->OutPort = this->InPort;
        }else{
            this->InPort  = std::make_shared<Port>(this->m_stall,this->m_flush);
            this->OutPort = std::make_shared<Port>(this->m_stall,this->m_flush);
        }
        this->reset();
    };

    ~TimeBuffer(){};

    void stall(){
        *this->m_stall = true;
    };
    bool isStalled(){
        return *this->m_stall;
    };
    bool isDrained(){
        return this->m_dataQueue.empty() && !this->OutPort->valid;
    };
    void flush(){
        *this->m_flush  = true;
    };
    void reset(){
        *this->m_flush   = false;
        *this->m_stall   = false;
        this->OutPort->kill();
        this->InPort->kill();
        while(!this->m_dataQueue.empty()){
            this->m_dataQueue.pop();
        }
    };
    void advance(){
        if(!*this->m_flush){
            if(!*this->m_stall){
                if(this->m_latency && this->InPort->valid){
                    //减一的目的是为了让数据在被用之前，就已经在ouput端口等待使用了，而不是要使用的时候，才开始传输数据到output
                    //比如this->m_latency=1，那么数据被push到队列后马上就被set到out端口，这样下一个周期开始时，就可以直接调用out端口的数据了
                    this->m_dataQueue.push({this->InPort->data,Clock::CurTick()+this->m_latency-1});
                    this->InPort->kill();
                }
                this->OutPort->kill();
                if(this->m_latency && !this->m_dataQueue.empty() && this->m_dataQueue.front().DestTick <= Clock::CurTick()){
                    this->OutPort->set(this->m_dataQueue.front().data);
                    this->m_dataQueue.pop();
                }
            }else{
                *this->m_stall = false;
            }
        }else{
            this->reset();//清空整个队列，杀死端口
        }
    };
public:
    std::shared_ptr<Port> InPort;
    std::shared_ptr<Port> OutPort;
private:
    std::shared_ptr<bool> m_stall;
    std::shared_ptr<bool> m_flush;

    timebuf_t             m_dataQueue;
    const uint64_t        m_latency;
};
} // namespace Emulator




#endif	