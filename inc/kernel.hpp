#ifndef PROJECT_KERNEL_HPP
#define PROJECT_KERNEL_HPP
#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/syscall_cpp.hpp"

void mem_init();
void* mem_alloc_k (size_t size);
int mem_free_k (void* addr);
int thread_create_k(thread_t* handle, void (*start_routine)(void*), void* arg, void* stack);
int thread_exit_k();
void thread_dispatch_k();
void thread_join_k(thread_t handle);
int sem_open_k(sem_t* handle, unsigned init);
int sem_close_k (sem_t handle);
int sem_wait_k (sem_t id);
int sem_signal_k (sem_t id);
void putc_k(char c);
char getc_k();
#endif