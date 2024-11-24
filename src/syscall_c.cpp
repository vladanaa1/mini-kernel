
#include "../h/syscall_c.hpp"
#include "../h/print.hpp"


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
    __asm__ volatile ("mv a3, %[temp]" : : [temp] "r" (arg)); // mv	a3,a2
    __asm__ volatile ("mv a2, %[temp]" : : [temp] "r" (start_routine)); // mv	a2,a1
    __asm__ volatile ("mv a1, %[temp]" : : [temp] "r" (handle)); // mv	a1,a0
    __asm__ volatile ("li a0, 0x11"); // li a0, 0x11

    __asm__ volatile ("ecall");

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
    __asm__ volatile("mv a2, %0" : : "r"(init));
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile ("li a0, 0x21");

    __asm__ volatile ("ecall");

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int)ret;
}

int sem_close(sem_t id) {

    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile ("li a0, 0x22");

    __asm__ volatile ("ecall");

    //syscall(SEM_CLOSE, (void*) pointer);

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int)ret;
}

int sem_wait(sem_t id) {

    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile ("li a0, 0x23");

    __asm__ volatile ("ecall");

    //syscall(SEM_WAIT, (void*)pointer);

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int)ret;
}

int sem_signal(sem_t id) {

    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile ("li a0, 0x24");

    __asm__ volatile ("ecall");

    //syscall(SEM_SIGNAL, (void*)pointer);

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int)ret;
}

int sem_timedwait(sem_t id, time_t timeout) {

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

    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile ("li a0, 0x26");

    __asm__ volatile ("ecall");

    //syscall(SEM_TRYWAIT, (void*)pointer);

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int)ret;
}

int time_sleep(time_t time) {

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

