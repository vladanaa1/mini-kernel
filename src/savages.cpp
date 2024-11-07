//
// Created by os on 8/12/24.
//
#include "../h/savages.hpp"
#include "../h/_sem.hpp"
#include "../h/print.hpp"
#include "../h/syscall_c.hpp"

const int M = 12;
int pot = 0;

_sem* mutex;
_sem* cookS;
_sem* savageS;


void cook(void*){
    printString1("cook function called\n");
    //mutex = _sem::_sem_open(1);
    sem_open(&mutex, 1); // -> radi
    //printInteger(res);
    sem_open(&cookS,0);
    sem_open(&savageS,0);
    while(true) {
        //uint64 res = _sem::_sem_timedwait(cookS, 0); // -> radi, vraca uvek 0?
        //printInteger(res);
        //printString1("\n");
        sem_wait(cookS); // -> radi
        //_sem::_sem_wait(cookS);
        printString1("Cook has prepared food for savages!\n");
        pot = M;
        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) {
                // busy wait
            }
        }
        sem_signal(savageS);
        //_sem::_sem_signal(savageS);
        /*
        uint64 res = time_sleep(600);
        printString("\n");
        printInteger(res);
        printString1("\n");
        */
    }
}

void savage(void* id){
    printString1("savage function called\n");
    int i = *(int*)id;
    while(true){
        sem_wait(mutex);
        //_sem::_sem_wait(mutex);
        if(pot == 0){
            sem_signal(cookS);
            sem_wait(savageS);
            //_sem::_sem_signal(cookS);
            //_sem::_sem_wait(savageS);
        }
        printString1("Savage ");
        printInteger(i);
        printString1(" is eating!\n");
        pot--;
        sem_signal(mutex);
        //_sem::_sem_signal(mutex);
        for (uint64 j = 0; j < 10000; j++)
        {
            for (uint64 k = 0; k < 30000; k++) {
                // busy wait
            }
        }
    }
}
