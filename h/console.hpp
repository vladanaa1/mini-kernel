//
// Created by os on 8/28/24.
//

#ifndef OS_VERZIJA_POSLE_MORA_CONSOLE_HPP
#define OS_VERZIJA_POSLE_MORA_CONSOLE_HPP

#include "../h/_sem.hpp"


class IO{
private:
    static char *InputBuffer, *OutputBuffer;

    static uint64 InputHead, OutputHead,
    InputTail, OutputTail, size;

    static _sem* mutexHeadInput, *mutexTailOutput, *mutexHeadOutput, *mutexTailInput,
    *fullInput, *emptyInput,
    *fullOutput, *emptyOutput;

public:
    static void putIntoInputBuffer(char c);

    static char getFromInputBuffer();

    static void putIntoOutputBuffer(char c);

    static char getFromOutputBuffer();

    static bool is_empty();

    static void initialize();
};

#endif //OS_VERZIJA_POSLE_MORA_CONSOLE_HPP

