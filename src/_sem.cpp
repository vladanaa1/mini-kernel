//
// Created by os on 8/12/24.
//

#include "../h/_sem.hpp"
#include "../h/syscall_c.hpp"
#include "../h/print.hpp"

_sem* _sem::_sem_open(unsigned int init) {
    _sem* sem = (_sem*) (new _sem(init));
    return sem;
}

uint64 _sem::_sem_close(_sem* id) {
    id->open = 0;
    TCB* thread = id->blocked.removeFirst();
    while(thread){
        thread->setBlocked(false);
        Scheduler::put(thread);
        thread = id->blocked.removeFirst();
    }
    printString1("sem closed\n");
    return 0;
}

uint64 _sem::_sem_wait(_sem* id) {
    if(!id->open) return -1;
    if((int)--(id->value) < 0){
        TCB::running->setBlocked(true);
        id->blocked.addLast(TCB::running);
        //TCB::yield();
        thread_dispatch();
    }
    if(!id->open) return -1;
    return 0;
}

uint64 _sem::_sem_signal(_sem* id) {
    if(!id->open) return -1;
    if((int)++(id->value) <= 0){
        TCB* thread = id->blocked.removeFirst();
        thread->setBlocked(false);
        Scheduler::put(thread);
    }
    return 0;
}

uint64 _sem::_sem_timedwait(_sem* id, time_t timeout) {
    uint64 initial_time = TCB::getTime();
    if(--id->value<0){
        TCB::running->setBlocked(true);
        id->blocked.addLast(TCB::running);
        thread_dispatch();
    }
    uint64 deblock_time = TCB::getTime();
    if(id->open){
        if(deblock_time - initial_time <= timeout){
            return 0; // OK
        }
        else return -2; // TIMEOUT
    }
    else return -1; // SEMDEAD
}

uint64 _sem::_sem_trywait(_sem* id) {
    if(id->value>0){
        id->value--;
        return 1;
    }
    else if(id->value==0){
        id->value--;
        return 0;
    }
    else {
        return -1;
    }
}

/*
void _sem::block() {
    blocked.addLast(TCB::running);
    TCB::running->setBlocked(true);
    thread_dispatch();
}

void _sem::deblock() {
    TCB* thread = blocked.removeFirst();
    thread->setBlocked(false);
    Scheduler::put(thread);
}
*/
