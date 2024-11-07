//
// Created by os on 8/10/24.
//

#ifndef OS_VERZIJA_POSLE_MORA_TCB_HPP
#define OS_VERZIJA_POSLE_MORA_TCB_HPP

class BlockedQueue;

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "blocked_queue.hpp"

// Thread Control Block
class TCB
{
public:
    ~TCB() { delete[] stack; }

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    bool isBlocked() const { return blocked; }

    void setBlocked(bool value) { blocked = value; }

    uint64 getTimeSlice() const { return timeSlice; }

    static uint64 getTime() { return time; }

    using Body = void (*)(void*);

    //using Body = void (*)();

    static TCB *createThread(Body body, void* args);

    // static TCB *createThread(Body body);

    static void thread_exit();

    static void yield();

    uint64 time_sleep(uint64 time);

    static TCB *running;

private:
    TCB(Body body, void* args, uint64 timeSlice) :
            body(body),
            args(args),
            stack(body != nullptr ? new uint64[STACK_SIZE] : nullptr),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
                    }),
            timeSlice(timeSlice),
            finished(false),
            blocked(false)
    {
        if (body != nullptr) { Scheduler::put(this); }
    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    void* args;
    uint64 *stack;
    Context context;
    uint64 timeSlice; // broj perioda tajmera koliko se nit izvrsava svaki put kad dobije procesor
    bool finished;
    bool blocked;

    friend class RiscV;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    static uint64 timeSliceCounter;
    static uint64 time;

    static BlockedQueue* blockedOnTimeSleep;

    static uint64 constexpr STACK_SIZE = 1024;
    static uint64 constexpr TIME_SLICE = 2;
};


#endif //OS_VERZIJA_POSLE_MORA_TCB_HPP
