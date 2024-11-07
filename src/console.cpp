//
// Created by os on 8/28/24.
//
#include "../h/console.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/print.hpp"

char* IO::InputBuffer = new char[512];
char* IO::OutputBuffer = new char[512];

uint64 IO::InputHead = 0;
uint64 IO::OutputHead = 0;
uint64 IO::InputTail = 0;
uint64 IO::OutputTail = 0;

uint64 IO::size = 512;

_sem* IO::fullInput = _sem::_sem_open(0);
_sem* IO::emptyInput = _sem::_sem_open(512);

_sem* IO::fullOutput = _sem::_sem_open(0);
_sem* IO::emptyOutput = _sem::_sem_open(512);

_sem* IO::mutexTailInput = _sem::_sem_open(1);
_sem* IO::mutexHeadInput = _sem::_sem_open(1);

_sem* IO::mutexTailOutput = _sem::_sem_open(1);
_sem* IO::mutexHeadOutput = _sem::_sem_open(1);


void IO::putIntoInputBuffer(char c) {
    _sem::_sem_wait(emptyInput);

    _sem::_sem_wait(mutexTailInput);
    InputBuffer[InputTail] = c;
    InputTail = (InputTail + 1) % size;
    _sem::_sem_signal(mutexTailInput);

    _sem::_sem_signal(fullInput);
}

char IO::getFromInputBuffer() {
    _sem::_sem_wait(fullInput);

    _sem::_sem_wait(mutexHeadInput);
    char ret = InputBuffer[InputHead];
    InputHead = (InputHead + 1) % size;
    _sem::_sem_signal(mutexHeadInput);

    _sem::_sem_signal(emptyInput);

    return ret;
}


void IO::putIntoOutputBuffer(char c) {
    _sem::_sem_wait(emptyOutput);

    _sem::_sem_wait(mutexTailOutput);
    OutputBuffer[OutputTail] = c;
    OutputTail = (OutputTail + 1) % size;
    _sem::_sem_signal(mutexTailOutput);

    _sem::_sem_signal(fullOutput);
}

char IO::getFromOutputBuffer() {
    _sem::_sem_wait(fullOutput);

    _sem::_sem_wait(mutexHeadOutput);
    char ret = OutputBuffer[OutputHead];
    OutputHead = (OutputHead + 1) % size;
    _sem::_sem_signal(mutexHeadOutput);

    _sem::_sem_signal(emptyOutput);

    return ret;
}

bool IO::is_empty() {
    return OutputTail == OutputHead;
}

void IO::initialize(){
    InputBuffer = new char[512];
    OutputBuffer = new char[512];

    InputHead = 0;
    OutputHead = 0;
    InputTail = 0;
    OutputTail = 0;

    size = 512;

    fullInput = _sem::_sem_open(0);
    emptyInput = _sem::_sem_open(512);

    fullOutput = _sem::_sem_open(0);
    emptyOutput = _sem::_sem_open(512);

    mutexTailInput = _sem::_sem_open(1);
    mutexHeadInput = _sem::_sem_open(1);

    mutexTailOutput = _sem::_sem_open(1);
    mutexHeadOutput = _sem::_sem_open(1);
}

