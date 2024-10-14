#include "../h/syscall_c.hpp"
#include "../lib/console.h"

void switchToUserMode(){
    uint64 oper=0x03;
    __asm__ volatile("mv a0,%[oc]" : :[oc]"r" (oper));
    __asm__ volatile ("ecall");
    return;
}

void* mem_alloc (size_t size){
    uint64 oper=0x01;
    size_t sz=(size+ MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE;
    __asm__ volatile ("mv a1,%[op]" : :[op]"r" (sz));
    __asm__ volatile("mv a0,%[oc]" : :[oc]"r" (oper));
    __asm__ volatile ("ecall");
    uint64 x;
    __asm__ volatile("mv %[rx1], a0" : [rx1]"=r" (x));
    return (void*) x;
}

int mem_free (void* addr){
    uint64 oper=0x02;
    __asm__ volatile ("mv a1,%[op]" : :[op]"r" (addr));
    __asm__ volatile("mv a0,%[oc]" : :[oc]"r" (oper));
    __asm__ volatile ("ecall");
    uint64 x;
    __asm__ volatile("mv %[rx1], a0" : [rx1]"=r" (x));
    return (int) x;
    // return mem_free_abi(addr);
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg){

    // ALOKACIJA STEKA
    void* stack = mem_alloc(DEFAULT_STACK_SIZE);

    uint64 oper=0x11;
    __asm__ volatile ("mv a4,%[op]" : :[op]"r" (stack));
    __asm__ volatile ("mv a3,%[op]" : :[op]"r" (arg));
    __asm__ volatile ("mv a2,%[op]" : :[op]"r" (start_routine));
    __asm__ volatile ("mv a1,%[op]" : :[op]"r" (handle));
    __asm__ volatile("mv a0,%[oc]" : :[oc]"r" (oper));
    __asm__ volatile ("ecall");
    uint64 x;
    __asm__ volatile("mv %[rx1], a0" : [rx1]"=r" (x));
    return (int) x;
}

int thread_exit (){
    uint64 oper=0x12;
    __asm__ volatile("mv a0,%[oc]" : :[oc]"r" (oper));
    __asm__ volatile ("ecall");
    uint64 x;
    __asm__ volatile("mv %[rx1], a0" : [rx1]"=r" (x));
    return (int) x;
}

void thread_dispatch (){
    uint64 oper=0x13;
    __asm__ volatile("mv a0,%[oc]" : :[oc]"r" (oper));
    __asm__ volatile ("ecall");
    return;
}

void thread_join (thread_t handle){
    uint64 oper=0x14;
    __asm__ volatile ("mv a1,%[op]" : :[op]"r" (handle));
    __asm__ volatile("mv a0,%[oc]" : :[oc]"r" (oper));
    __asm__ volatile ("ecall");
    return;
}

int sem_open(sem_t* handle, unsigned init){
    uint64 oper=0x21;
    __asm__ volatile ("mv a2,%[op]" : :[op]"r" (init));
    __asm__ volatile ("mv a1,%[op]" : :[op]"r" (handle));
    __asm__ volatile("mv a0,%[oc]" : :[oc]"r" (oper));
    __asm__ volatile ("ecall");
    uint64 x;
    __asm__ volatile("mv %[rx1], a0" : [rx1]"=r" (x));
    return (int) x;
}

int sem_close (sem_t handle){
    uint64 oper=0x22;
    __asm__ volatile ("mv a1,%[op]" : :[op]"r" (handle));
    __asm__ volatile("mv a0,%[oc]" : :[oc]"r" (oper));
    __asm__ volatile ("ecall");
    uint64 x;
    __asm__ volatile("mv %[rx1], a0" : [rx1]"=r" (x));
    return (int) x;
}

int sem_wait (sem_t id){
    uint64 oper=0x23;
    __asm__ volatile ("mv a1,%[op]" : :[op]"r" (id));
    __asm__ volatile("mv a0,%[oc]" : :[oc]"r" (oper));
    __asm__ volatile ("ecall");
    uint64 x;
    __asm__ volatile("mv %[rx1], a0" : [rx1]"=r" (x));
    return (int) x;
}

int sem_signal (sem_t id){
    uint64 oper=0x24;
    __asm__ volatile ("mv a1,%[op]" : :[op]"r" (id));
    __asm__ volatile("mv a0,%[oc]" : :[oc]"r" (oper));
    __asm__ volatile ("ecall");
    uint64 x;
    __asm__ volatile("mv %[rx1], a0" : [rx1]"=r" (x));
    return (int) x;
}

char getc(){
    uint64 oper=0x41;
    __asm__ volatile("mv a0,%[oc]" : :[oc]"r" (oper));
    __asm__ volatile ("ecall");
    uint64 x;
    __asm__ volatile("mv %[rx1], a0" : [rx1]"=r" (x));
    return (char) x;
    // return __getc();
}
void putc(char c){
    uint64 oper=0x42;
    __asm__ volatile ("mv a1,%[op]" : :[op]"r" (c));
    __asm__ volatile("mv a0,%[oc]" : :[oc]"r" (oper));
    __asm__ volatile ("ecall");
    return;
    // __putc(c);
}

