//
// Created by os on 8/10/24.
//

#ifndef OS_VERZIJA_POSLE_MORA_LIST_HPP
#define OS_VERZIJA_POSLE_MORA_LIST_HPP

template<typename T>
class List {
private:
    struct Elem{
        T* data;
        Elem* next, *prev;

        Elem(T* data, Elem *next = nullptr, Elem* prev = nullptr): data(data), next(next), prev(prev){}
    };

    Elem * head, *tail, *cur;

public:
    List(): head(0), tail(0) , cur(0){}

    void addLast(T* data){
        Elem * elem = new Elem(data,0);
        if(tail){
            tail->next = elem;
            elem->prev = tail;
            tail = elem;
        }
        else{
            head = tail = elem;
            cur = head;
        }
    }

    T* removeFirst(){
        if(!head){return 0;}

        Elem* elem = head;
        head = head->next;
        if(!head) {tail = 0;}

        if(elem == cur){
            cur = head;
        }

        T* ret = elem->data;
        delete elem;
        return ret;
    }

    T* getFirst(){
        return head ? head->data : nullptr;
    }




};

#endif //OS_VERZIJA_POSLE_MORA_LIST_HPP
