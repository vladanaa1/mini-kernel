//
// Created by os on 8/10/24.
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/print.hpp"

TCB* TCB::running = nullptr;

uint64 TCB::timeSliceCounter = 0;
uint64 TCB::time = 0;

BlockedQueue* TCB::blockedOnTimeSleep = new BlockedQueue();

void TCB::yield(){
    __asm__ volatile("mv a0, %0" : : "r"(THREAD_DISPATCH));
    __asm__ volatile ("ecall");
}

void TCB::dispatch(){
    TCB* old = running;
    if(!old->isFinished() && !old->isBlocked()) {
        Scheduler::put(old);
    }
    running = Scheduler::get();

    contextSwitch(&old->context, &running->context);
}


TCB *TCB::createThread(TCB::Body body, void* args) {
    return new TCB(body, args, TIME_SLICE);
}

/*
TCB *TCB::createThread(TCB::Body body) {
    return new TCB(body, TIME_SLICE);
}
*/

void TCB::threadWrapper()
{
    RiscV::popSppSpie(); // spp = supervisor previous privilege, spie = previous interrupt enable
    running->body(running->args);
    running->setFinished(true); // mora eksplicitno da se postavi finished
    TCB::yield();
}

void TCB::thread_exit() {
    TCB* old = running;
    old->setFinished(true);
    running = Scheduler::get();
    contextSwitch(&old->context, &running->context);
}

uint64 TCB::time_sleep(uint64 time) {
    uint64 wake_up_time = TCB::getTime() + time;
    TCB::running->setBlocked(true);
    if(!blockedOnTimeSleep) blockedOnTimeSleep = new BlockedQueue();
    blockedOnTimeSleep->push(TCB::running, wake_up_time);
    /*
    printString("current time is ");
    printInteger(TCB::getTime());
    printString("\n");
    printString("wakeup time is ");
    printInteger(wake_up_time);
    printString1("\n");
    */
    TCB::yield();

    if(TCB::getTime() >= wake_up_time) return 0; // OK
    else return -1; // ERROR
}

