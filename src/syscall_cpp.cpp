//
// Created by os on 8/13/24.
//


#include "../h/syscall_cpp.hpp"
#include "../h/riscv.hpp"
#include "../lib/console.h"
#include "../h/print.hpp"


Thread::Thread(void (*body)(void *), void *arg) : body(body), arg(arg) {
    myHandle = nullptr;
    //printString1("Constructor called\n");
}

int Thread::start() {
    return thread_create(&myHandle, threadWrapper, this);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t time) {
    return time_sleep(time);
}

Thread::~Thread() {
    //thread_exit(); // -> ovo sig ne moze
    myHandle->setFinished(true);
    delete myHandle;
}

Thread::Thread() {
    myHandle = nullptr;
    body = nullptr;
    arg = nullptr;
}

void Thread::threadWrapper(void *thr) {
    printString1("ThreadWrapper called!\n");
    Thread* thread = (Thread*) thr;
    if(!thread->body){
        printString1("ovo\n");
        thread->run();
    }
    else{
        thread->body(thread->arg);
    }
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

int Semaphore::timedWait(time_t time) {
    return sem_timedwait(myHandle, time);
}

int Semaphore::tryWait() {
    return sem_trywait(myHandle);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}


PeriodicThread::PeriodicThread(time_t period) : period(period) {}

void PeriodicThread::run(){
    while(period){
        periodicActivation(); // korisnik sam redefinise periodicActivation
        if(!period){
            break;
        }
        time_sleep(period);
    }
}

void PeriodicThread::terminate() {
    period = 0;
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}
