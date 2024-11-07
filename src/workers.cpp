#include "../lib/hw.h"
#include "../h/tcb.hpp"
#include "../h/print.hpp"

#include "../h/syscall_c.hpp"

void workerBodyA(void*)
{
    //thread_exit();
    for (uint64 i = 0; i < 10; i++)
    {
        printString1("A: i=");
        printInteger(i);
        printString1("\n");
        for (uint64 j = 0; j < 10000; j++)
        {
            for (uint64 k = 0; k < 30000; k++)
            {
                // busy wait
            }
//            TCB::yield(); -> ovo je sa preotimanjem, nigde nemamo eksplicitno yield
        }
    }
}

void workerBodyB(void*)
{
    for (uint64 i = 0; i < 16; i++)
    {
        printString1("B: i=");
        printInteger(i);
        printString1("\n");
        for (uint64 j = 0; j < 10000; j++)
        {
            for (uint64 k = 0; k < 30000; k++)
            {
                // busy wait
            }
//            TCB::yield();
        }
    }
}

static uint64 fibonacci(uint64 n)
{
    if (n == 0 || n == 1) { return n; }
    if (n % 10 == 0) { TCB::yield(); }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void workerBodyC(void*)
{
    uint8 i = 0;
    for (; i < 3; i++)
    {
        printString1("C: i=");
        printInteger(i);
        printString1("\n");
    }

    printString1("C: yield\n");
    __asm__ ("li t1, 7");
    TCB::yield();

    uint64 t1 = 0;
    __asm__ ("mv %[t1], t1" : [t1] "=r"(t1));

    printString1("C: t1=");
    printInteger(t1);
    printString1("\n");

    uint64 result = fibonacci(12);
    printString1("C: fibonaci=");
    printInteger(result);
    printString1("\n");

    for (; i < 6; i++)
    {
        printString1("C: i=");
        printInteger(i);
        printString1("\n");
    }
//    TCB::yield();
}

void workerBodyD(void*)
{
    uint8 i = 10;
    for (; i < 13; i++)
    {
        printString1("D: i=");
        printInteger(i);
        printString1("\n");
    }

    printString1("D: yield\n");
    __asm__ ("li t1, 5");
    TCB::yield();

    uint64 result = fibonacci(16);
    printString1("D: fibonaci=");
    printInteger(result);
    printString1("\n");

    for (; i < 16; i++)
    {
        printString1("D: i=");
        printInteger(i);
        printString1("\n");
    }
//    TCB::yield();
}

void workerBodyE(void*){
    for (uint64 i = 0; i < 5; i++)
    {
        printString1("E: i=");
        printInteger(i);
        printString1("\n");
        for (uint64 j = 0; j < 10000; j++)
        {
            for (uint64 k = 0; k < 30000; k++)
            {
                // busy wait
            }
//            TCB::yield(); -> ovo je sa preotimanjem, nigde nemamo eksplicitno yield
        }
    }
    //thread_exit(); // -> radi
    time_sleep(100);
    for (uint64 i = 5; i < 9; i++) {
        printString1("E: i=");
        printInteger(i);
        printString1("\n");
        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) {
                // busy wait
            }
//            TCB::yield(); -> ovo je sa preotimanjem, nigde nemamo eksplicitno yield
        }
    }
}

void workerBodyF(void*){
    for (uint64 i = 0; i < 10; i++) {
        printString1("F: i=");
        printInteger(i);
        printString1("\n");
        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) {
                // busy wait
            }
//            TCB::yield(); -> ovo je sa preotimanjem, nigde nemamo eksplicitno yield
        }
    }
}