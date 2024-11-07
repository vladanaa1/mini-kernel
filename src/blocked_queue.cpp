//
// Created by os on 8/12/24.
//
#include "../h/blocked_queue.hpp"
#include "../h/print.hpp"

void BlockedQueue::push(TCB *tcb, uint64 wake_up_time) {
    Blocked* elem = new Blocked(tcb, wake_up_time);
    if(tail){
        tail->next = elem;
        tail = elem;
    }
    else{
        head = tail = elem;
    }
}

void BlockedQueue::iterate() {
    //printString1("iterate function called\n");
    Blocked* current = head;
    Blocked* prev = nullptr;
    while(current){
        if(TCB::getTime() >= current->wake_up_time){
            //printString1("current time is ");
            //printInteger(TCB::getTime());
            //printString1("\n");
            //printString1("wake up time is ");
            //printInteger(current->wake_up_time);
            //printString1("\n");
            if(prev){
                prev->next = current->next;
            }
            else{
                head = current->next;
            }
            if(current == tail){
                tail = prev;
            }
            Blocked* temp = current;
            current = current->next;
            temp->tcb->setBlocked(false);
            Scheduler::put(temp->tcb);
            delete temp;
        }
        else{
            prev = current;
            current = current->next;
        }
    }
}

