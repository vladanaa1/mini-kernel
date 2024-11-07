//
// Created by os on 8/13/24.
//

#ifndef OS_VERZIJA_POSLE_MORA_SYSCALL_CPP_HPP
#define OS_VERZIJA_POSLE_MORA_SYSCALL_CPP_HPP

#include "syscall_c.hpp"


class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    static void dispatch ();
    static int sleep (time_t time);
protected:
    Thread ();
    static void threadWrapper(void* thr);
    virtual void run () {}
private:
    thread_t myHandle;
    void (*body)(void*); void* arg;
};


class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
    int timedWait (time_t time);
    int tryWait();
private:
    sem_t myHandle;
};

class PeriodicThread : public Thread {
public:
    void terminate ();
protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {}
private:
    virtual void run() override;
    time_t period;
};

class Console {
public:
    static char getc();
    static void putc(char);
};

#endif //OS_VERZIJA_POSLE_MORA_SYSCALL_CPP_HPP
