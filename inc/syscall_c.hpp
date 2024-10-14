#ifndef PROJECT_SYSCALL_C_H
#define PROJECT_SYSCALL_C_H
#include "../lib/hw.h"

class TCB;
typedef TCB* thread_t;

class _sem;
typedef _sem* sem_t;
// extern "C" void* mem_alloc_abi (size_t size);
// extern "C" int mem_free_abi (void*);

void switchToUserMode();

void* mem_alloc (size_t size);

int mem_free (void*);
// Thread
int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);

int thread_exit ();

void thread_dispatch ();

void thread_join (thread_t handle);
// Semaphore
int sem_open(sem_t* handle, unsigned init);
int sem_close (sem_t handle);
int sem_wait (sem_t id);
int sem_signal (sem_t id);

const int EOF = -1;
char getc();
void putc(char c);
#endif