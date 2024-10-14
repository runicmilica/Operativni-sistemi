#ifndef PROJECT_SYSCALL_CPP
#define PROJECT_SYSCALL_CPP
#include "syscall_c.hpp"
#include "../lib/hw.h"

void* operator new (size_t);
void operator delete (void*);

void* operator new[](uint64);
void operator delete[](void*);

class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    void join();
    static void dispatch ();
    static int sleep (time_t);
protected:
    Thread ();
    virtual void run () {}
private:
    thread_t myHandle;
    void (*body)(void*);
    void* arg;
    friend class TCB;
};

class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
private:
    sem_t myHandle;
};

class Console {
public:
    static char getc ();
    static void putc (char);
};

#endif