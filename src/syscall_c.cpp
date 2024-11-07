
#include "../h/syscall_c.hpp"
#include "../h/print.hpp"
/*
struct thread_create_params{
    thread_t *handle;
    void (*start_routine)(void *);
    void *arg;
};

struct sem_open_params{
    sem_t *handle;
    unsigned int init;
};

struct sem_params{
    sem_t handle;
};

struct sem_timedwait_params{
    sem_t handle;
    time_t timeout;
};

struct time_sleep_params{
    time_t time;
};
 */

void* mem_alloc (size_t size){
    __asm__ volatile ("mv a1, %[temp]" : : [temp] "r" (size));
    __asm__ volatile ("li a0, 0x01");

    __asm__ volatile ("ecall");

    void* volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int mem_free (void* p){
    __asm__ volatile ("mv a1, %[temp]" : : [temp] "r" (p));
    __asm__ volatile ("li a0, 0x02");

    __asm__ volatile ("ecall");

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int)ret;
}

int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg) { // a0, a1, a2
    //thread_create_params params = {handle, start_routine, arg};
    //thread_create_params* pointer = &params;
    //printString1("thread_create sys call called!\n");
    __asm__ volatile ("mv a3, %[temp]" : : [temp] "r" (arg)); // mv	a3,a2
    __asm__ volatile ("mv a2, %[temp]" : : [temp] "r" (start_routine)); // mv	a2,a1
    __asm__ volatile ("mv a1, %[temp]" : : [temp] "r" (handle)); // mv	a1,a0
    __asm__ volatile ("li a0, 0x11"); // li a0, 0x11

    __asm__ volatile ("ecall");

    //syscall(THREAD_CREATE, (void*) pointer);

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int)ret;
}

int thread_exit() {
    __asm__ volatile ("li a0, 0x12"); // li a0, 0x12

    __asm__ volatile ("ecall");

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int)ret;
}

void thread_dispatch() {
    __asm__ volatile ("li a0, 0x13");
    __asm__ volatile ("ecall");
}

int sem_open(sem_t *handle, unsigned int init) {
    //sem_open_params params = {handle, init};
    //sem_open_params* pointer = &params;
    __asm__ volatile("mv a2, %0" : : "r"(init));
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile ("li a0, 0x21");

    __asm__ volatile ("ecall");

    //syscall(SEM_OPEN, (void*)pointer);

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int)ret;
}

int sem_close(sem_t id) {
    //sem_params params = {id};
    //sem_params* pointer = &params;

    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile ("li a0, 0x22");

    __asm__ volatile ("ecall");

    //syscall(SEM_CLOSE, (void*) pointer);

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int)ret;
}

int sem_wait(sem_t id) {
    //sem_params params = {id};
    //sem_params* pointer = &params;

    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile ("li a0, 0x23");

    __asm__ volatile ("ecall");

    //syscall(SEM_WAIT, (void*)pointer);

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int)ret;
}

int sem_signal(sem_t id) {
    //sem_params params = {id};
    //sem_params* pointer = &params;

    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile ("li a0, 0x24");

    __asm__ volatile ("ecall");

    //syscall(SEM_SIGNAL, (void*)pointer);

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int)ret;
}

int sem_timedwait(sem_t id, time_t timeout) {
    //sem_timedwait_params params = {id, timeout};
    //sem_timedwait_params* pointer = &params;

    __asm__ volatile("mv a2, %0" : : "r"(timeout));
    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile ("li a0, 0x25");

    __asm__ volatile ("ecall");

    //syscall(SEM_TIMEDWAIT, (void*)pointer);

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int)ret;
}

int sem_trywait(sem_t id) {
    //sem_params params = {id};
    //sem_params* pointer = &params;

    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile ("li a0, 0x26");

    __asm__ volatile ("ecall");

    //syscall(SEM_TRYWAIT, (void*)pointer);

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int)ret;
}

int time_sleep(time_t time) {
    //time_sleep_params params = {time};
    //time_sleep_params* pointer = &params;

    __asm__ volatile("mv a1, %0" : : "r"(time));
    __asm__ volatile ("li a0, 0x31");

    __asm__ volatile ("ecall");

    //syscall(TIME_SLEEP, (void*) pointer);

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int)ret;
}

char getc(){
    __asm__ volatile ("li a0, 0x41");
    __asm__ volatile ("ecall");

    char volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int)ret;
}



void putc(char c){
    __asm__ volatile("mv a1, %0" : : "r"(c));
    __asm__ volatile ("li a0, 0x42");

    __asm__ volatile ("ecall");
}

/*
void syscall(uint64 syscall_code, void *params) {
    //printString1("syscall function called\n");
    __asm__ volatile("mv a0, %0" : : "r"(syscall_code));

    thread_create_params* volatile params1 = nullptr;
    thread_t* volatile tcb = nullptr;
    void (*start_routine)(void *) = nullptr;
    void* volatile args = nullptr;

    sem_open_params* volatile params2 = nullptr;
    sem_t* volatile handle = nullptr;
    unsigned int volatile init = 0;

    sem_params* volatile params3 = nullptr;
    sem_t volatile id = nullptr;

    //sem_timedwait_params* volatile params4 = nullptr;
    //time_t volatile timeout;

    time_sleep_params* volatile params5 = nullptr;
    time_t volatile time;

    switch(syscall_code){
        case THREAD_CREATE:
            params1 = (thread_create_params*) params;
            tcb = params1->handle;
            start_routine = params1->start_routine;
            args = params1->arg;
            __asm__ volatile("mv a1, %0" : : "r"(tcb));
            __asm__ volatile("mv a2, %0" : : "r"(start_routine));
            __asm__ volatile("mv a3, %0" : : "r"(args));
            break;
        case SEM_OPEN:
            params2 = (sem_open_params*) params;
            handle = params2->handle;
            init = params2->init;
            __asm__ volatile("mv a1, %0" : : "r"(handle));
            __asm__ volatile("mv a2, %0" : : "r"(init));
            break;
        case SEM_CLOSE:
        case SEM_WAIT:
        case SEM_SIGNAL:
        //case SEM_TRYWAIT:
            params3 = (sem_params*) params;
            id = params3->handle;
            __asm__ volatile("mv a1, %0" : : "r"(id));
            break;

        case SEM_TIMEDWAIT: // -> kad odkomentarises, ne radi
            params4 = (sem_timedwait_params*) params;
            id = params4->handle;
            timeout = params4->timeout;
            __asm__ volatile("mv a1, %0" : : "r"(id));
            __asm__ volatile("mv a2, %0" : : "r"(timeout));
            break;

        case TIME_SLEEP:
            params5 = (time_sleep_params*) params;
            time = params5->time;
            __asm__ volatile("mv a1, %0" : : "r"(time));
            break;
    }
    __asm__ volatile ("ecall");
}
 */
