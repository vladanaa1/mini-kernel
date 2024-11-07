//
// Created by os on 8/12/24.
//

#ifndef OS_VERZIJA_POSLE_MORA__SEM_HPP
#define OS_VERZIJA_POSLE_MORA__SEM_HPP

#include "tcb.hpp"

class _sem {
public:
    static _sem* _sem_open(unsigned init);

    static uint64 _sem_close(_sem* id);

    static uint64 _sem_wait(_sem* id);

    static uint64 _sem_signal(_sem* id);

    static uint64 _sem_timedwait(_sem* id, time_t timeout);

    static uint64 _sem_trywait(_sem* id);

protected:
    void block();

    void deblock();

private:
    unsigned value;
    bool open;

    List<TCB> blocked;

    _sem(int initValue = 1) : value(initValue), open(0){
        open = 1;
    }

};

#endif //OS_VERZIJA_POSLE_MORA__SEM_HPP
