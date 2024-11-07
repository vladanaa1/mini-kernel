#include "../h/tcb.hpp"
#include "../h/workers.hpp"
#include "../h/savages.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"
#include "../h/console.hpp"
#include "../h/syscall_c.hpp"

extern void userMain();
void userMainWrapper(void*) {
    //printString1("userMainWrapper called\n");
    userMain();
}

int main()
{

    TCB *threads[3];

    threads[0] = TCB::createThread(nullptr, nullptr);
    TCB::running = threads[0];

    IO::initialize();

    RiscV::w_stvec((uint64) &RiscV::supervisorTrap);
    RiscV::ms_sstatus(RiscV::SSTATUS_SIE);



    threads[1] = TCB::createThread(userMainWrapper, nullptr);
    //threads[2] = TCB::createThread(idle, nullptr);
    //printString1("Thread 1 created\n");

    while(!threads[1]->isFinished()){
        TCB::yield();
    }

    for (auto &thread: threads)
    {
        delete thread;
    }

    return 0;

/*
    TCB *threads[5];

    //thread_create(&threads[0], nullptr, nullptr); -> ne radi, main ne prima argumente
    threads[0] = TCB::createThread(nullptr, nullptr);
    TCB::running = threads[0];

    RiscV::w_stvec((uint64) &RiscV::supervisorTrap);
    RiscV::ms_sstatus(RiscV::SSTATUS_SIE);

    //threads[1] = TCB::createThread(workerBodyA, nullptr);
    uint64 res = thread_create(&threads[1], workerBodyA, nullptr);
    printInteger(res); // nije dobro, printa se 17 sto je 0x11 sto je kod sistemskog poziva thread_create
    printString("\n");
    printString("ThreadA created\n");
    threads[2] = TCB::createThread(workerBodyB, nullptr);
    //thread_create(&threads[2], workerBodyB, nullptr);
    printString("ThreadB created\n");
    threads[3] = TCB::createThread(workerBodyC, nullptr);
    //thread_create(&threads[3], workerBodyC, nullptr);
    printString("ThreadC created\n");
    threads[4] = TCB::createThread(workerBodyD, nullptr);
    //thread_create(&threads[4], workerBodyD, nullptr);
    printString1("ThreadD created\n");



    while (!(threads[1]->isFinished() &&
             threads[2]->isFinished() &&
             threads[3]->isFinished() &&
             threads[4]->isFinished()))
    {
        thread_dispatch(); // -> radi, ispise ceo program, na kraju se ne zavrsi kernel kod jedino, ne znam sto kad je isti k
        //TCB::yield();
    }

    for (auto &thread: threads)
    {
        delete thread;
    }

*/
/*
    TCB *threads[5];

    uint64 id1 = 1;
    uint64 id2 = 2;
    uint64 id3 = 3;

    threads[0] = TCB::createThread(nullptr, nullptr);
    TCB::running = threads[0];

    RiscV::w_stvec((uint64) &RiscV::supervisorTrap);
    RiscV::ms_sstatus(RiscV::SSTATUS_SIE);

    threads[1] = TCB::createThread(cook, nullptr);
    //thread_create(&threads[1], cook, nullptr);
    //printInteger(ret);
    printString1("Cook created\n");
    //threads[2] = TCB::createThread(savage, (void*)&id1);
    thread_create(&threads[2], savage, &id1);
    printString1("Savage #1 created\n");
    threads[3] = TCB::createThread(savage, (void*)&id2);
    printString1("Savage #2 created\n");
    threads[4] = TCB::createThread(savage, (void*)&id3);
    printString1("Savage #3 created\n");

    while (!(threads[1]->isFinished() &&
             threads[2]->isFinished() &&
             threads[3]->isFinished() &&
             threads[4]->isFinished()
    ))
    {
        thread_dispatch();
        //TCB::yield();
    }

    for (auto &thread: threads)
    {
        delete thread;
    }

    printString1("Finished\n");

    return 0;
*/
/*
    TCB *threads[3];

    threads[0] = TCB::createThread(nullptr, nullptr);
    TCB::running = threads[0];

    RiscV::w_stvec((uint64) &RiscV::supervisorTrap);
    RiscV::ms_sstatus(RiscV::SSTATUS_SIE);

    thread_create(&threads[1], workerBodyE, nullptr);
    //threads[1] = TCB::createThread(workerBodyE, nullptr);
    printString("Thread E created\n");
    threads[2] = TCB::createThread(workerBodyF, nullptr);
    printString("Thread F created\n");

    while (!(threads[1]->isFinished() &&
             threads[2]->isFinished()
    ))
    {
        thread_dispatch();
        //TCB::yield();
    }

    for (auto &thread: threads)
    {
        delete thread;
    }

    printString1("Finished\n");

    return 0;
*/




}

