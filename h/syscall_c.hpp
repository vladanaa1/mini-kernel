//
// Created by os on 8/11/24.
//

#ifndef OS_VERZIJA_POSLE_MORA_SYSCALL_C_HPP
#define OS_VERZIJA_POSLE_MORA_SYSCALL_C_HPP

#include "../lib/hw.h"

#define THREAD_CREATE 0x11
#define THREAD_EXIT 0x12
#define THREAD_DISPATCH 0x13

#define SEM_OPEN 0x21
#define SEM_CLOSE 0x22
#define SEM_WAIT 0x23
#define SEM_SIGNAL 0x24
#define SEM_TIMEDWAIT 0x25
#define SEM_TRYWAIT 0x26

#define TIME_SLEEP 0x31

void syscall(uint64 syscall_code, void* params);

void* mem_alloc (size_t size);

int mem_free (void*);

class TCB;
typedef TCB* thread_t;

int thread_create(thread_t* handle,
                  void (*start_routine)(void*),
                  void* arg);

int thread_exit();

void thread_dispatch();

class _sem;
typedef _sem* sem_t;

int sem_open(
        sem_t* handle,
        unsigned init
);

int sem_close(sem_t handle);

int sem_wait(sem_t id);

int sem_signal(sem_t id);

typedef unsigned long time_t;

int sem_timedwait(
        sem_t id,
        time_t timeout
);

int sem_trywait(sem_t id);

int time_sleep(time_t time);

const int EOF = -1;
char getc ();

void putc (char);

#endif //OS_VERZIJA_POSLE_MORA_SYSCALL_C_HPP
