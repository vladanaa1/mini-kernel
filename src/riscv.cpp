//
// Created by os on 8/10/24.
//

#include "../h/riscv.hpp"

#include "../lib/console.h"
#include "../h/_sem.hpp"
#include "../h/print.hpp"
#include "../lib/mem.h"
#include "../h/console.hpp"

#include "../h/MemoryAllocator.hpp"

void RiscV::popSppSpie()
{
    ms_sstatus(SSTATUS_SPIE);
    __asm__ volatile ("csrw sepc, ra"); // sepc<=ra
    __asm__ volatile("csrc sstatus, %0" : : "r"(0x100));
    __asm__ volatile ("sret"); // pc<=sepc
}

void RiscV::handleSupervisorTrap()
{
    uint64 volatile syscall;
    __asm__ volatile("mv %0, a0" : "=r" (syscall));

    uint64 volatile a1;
    uint64 volatile a2;
    uint64 volatile a3;
    __asm__ volatile("mv %0, a1" : "=r" (a1));
    __asm__ volatile("mv %0, a2" : "=r" (a2));
    __asm__ volatile("mv %0, a3" : "=r" (a3));

    uint64 scause = r_scause(); // procitamo scause reg. kako bismo znali razlog prekida/izuzetka
    if (scause == 0x0000000000000009UL || scause == 0x0000000000000008UL)
    {
        // interrupt: no; cause code: environment from S-mode(9) or U-mode(8)
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();
        TCB::timeSliceCounter = 0;

        if(syscall == MEM_ALLOC){
            //void* ret = __mem_alloc((size_t )a1);
            void* ret = MemoryAllocator::allocate((uint64)a1);
            __asm__ volatile ("sd %0, 10*8(fp)" : : "r"(ret));
        }
        else if(syscall == MEM_FREE){
            //int ret = __mem_free((void*)a1);
            int ret = MemoryAllocator::deallocate((void*)a1);
            __asm__ volatile ("sd %0, 10*8(fp)" : : "r"(ret));
        }
        else if(syscall == THREAD_CREATE){
            *(TCB**)a1 = TCB::createThread((TCB::Body)a2, (void*)a3);
            int ret = 0;
            if(*(TCB**)a1==nullptr){
                ret = -1;
            }
            __asm__ volatile ("sd %0, 10*8(fp)" : : "r"(ret));
        }
        else if(syscall == THREAD_DISPATCH){
            TCB::dispatch();
        }
        else if(syscall == THREAD_EXIT){
            TCB::thread_exit();
            int ret = 0;
            __asm__ volatile ("sd %0, 10*8(fp)" : : "r"(ret));
        }
        else if(syscall == SEM_OPEN){
            *(_sem**) a1 = _sem::_sem_open((int)a2);
            int ret = 0;
            if(*(_sem**)a1){
                ret = 0;
            }
            else ret = -1;
            __asm__ volatile ("sd %0, 10*8(fp)" : : "r"(ret));
        }
        else if(syscall == SEM_CLOSE){
            uint64 ret = _sem::_sem_close((_sem*)a1);
            __asm__ volatile ("sd %0, 10*8(fp)" : : "r"(ret));
        }
        else if(syscall == SEM_WAIT){
            uint64 ret = _sem::_sem_wait((_sem*)a1);
            __asm__ volatile ("sd %0, 10*8(fp)" : : "r"(ret));
        }
        else if(syscall == SEM_SIGNAL){
            uint64 ret = _sem::_sem_signal((_sem*)a1);
            __asm__ volatile ("sd %0, 10*8(fp)" : : "r"(ret));
        }
        else if(syscall == SEM_TIMEDWAIT){
            uint64 ret = _sem::_sem_timedwait((_sem*)a1, a2);
            __asm__ volatile ("sd %0, 10*8(fp)" : : "r"(ret));
        }
        else if(syscall == SEM_TRYWAIT){
            uint64 ret = _sem::_sem_trywait((_sem*)a1);
            __asm__ volatile ("sd %0, 10*8(fp)" : : "r"(ret));
        }
        else if(syscall == GETC){
            //char c = __getc();
            char c = IO::getFromInputBuffer();
            __asm__ volatile ("sd %0, 10*8(fp)" : : "r"(c));
        }
        else if(syscall == PUTC){
            //__putc((char)a1);
            //printString1("putc system call called\n");
            IO::putIntoOutputBuffer((char)a1);
        }
        else if(syscall == TIME_SLEEP){ // 0x31
            uint64 ret = TCB::running->time_sleep(a1);
            __asm__ volatile ("sd %0, 10*8(fp)" : : "r"(ret));
        }

        //TCB::dispatch();

        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000001UL)
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        while ((*((char *) CONSOLE_STATUS) & CONSOLE_TX_STATUS_BIT) && !IO::is_empty()) {
            //printString1("machine timer interrupt && getting smth from output buffer\n");
            char c = IO::getFromOutputBuffer(); // nit jezgra je potrosac, korisnicka nit je proizvodjac
            // i ona "puni" InputBuffer sa putc
            *(volatile char *) CONSOLE_TX_DATA = c;
        }

        mc_sip(SIP_SSIE); // mask clear (interrupt enable/interrupt pending)
        TCB::time++;
        TCB::timeSliceCounter++;
        if(TCB::blockedOnTimeSleep)
        {
            if(!TCB::blockedOnTimeSleep->empty())
            {
                TCB::blockedOnTimeSleep->iterate();
            }
        }
        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
    }
    else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        // console_handler();

        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();

        // plic_claim - deklaracija data u hw.lib, vraca broj prekida u zavisnosti od uredjaja
        // ukoliko je prekid od konzole vraca 0x10
        uint64 interrupt = plic_claim();
        if(interrupt == CONSOLE_IRQ)
        {
            if ((*((char *) CONSOLE_STATUS) & CONSOLE_RX_STATUS_BIT))
            {
                volatile char c = *(char *) CONSOLE_RX_DATA;
                IO::putIntoInputBuffer(c); // prekidna rutina proizvodjac, kor. nit potrosac, u obradi sis. poziva
                // getc uzimamo iz InputBuffer-a
            }
        }
        plic_complete( CONSOLE_IRQ);
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else
    {
        // unexpected trap cause
    }
}