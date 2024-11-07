//
// Created by os on 8/12/24.
//

#ifndef OS_VERZIJA_POSLE_MORA_BLOCKED_QUEUE_HPP
#define OS_VERZIJA_POSLE_MORA_BLOCKED_QUEUE_HPP

#include "tcb.hpp"

class BlockedQueue{
private:
    struct Blocked{
        TCB* tcb;
        uint64 wake_up_time;
        Blocked *next;
        Blocked(TCB* tcb, uint64 wake_up_time, Blocked *next = nullptr):
                tcb(tcb),
                wake_up_time(wake_up_time),
                next(next) {}
    };

    static List<Blocked> blocked;

    Blocked *head, *tail;

    friend class TCB;

public:
    BlockedQueue(): head(0), tail(0) {}

    void push(TCB* tcb, uint64 wake_up_time);

    void iterate();

    // void pushFIFO(TCB* tcb, uint64 wake_up_time);

    // void iterateFIFO();

    bool empty() const { return head == nullptr; }

};

#endif //OS_VERZIJA_POSLE_MORA_BLOCKED_QUEUE_HPP
