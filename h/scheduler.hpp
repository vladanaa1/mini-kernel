//
// Created by os on 8/10/24.
//

#ifndef OS_VERZIJA_POSLE_MORA_SCHEDULER_HPP
#define OS_VERZIJA_POSLE_MORA_SCHEDULER_HPP

#include "list.hpp"

class TCB;

class Scheduler {
private:
    static List<TCB> readyCoroutineQueue;

public:
    static TCB* get();

    static void put(TCB *tcb);
};

#endif //OS_VERZIJA_POSLE_MORA_SCHEDULER_HPP
