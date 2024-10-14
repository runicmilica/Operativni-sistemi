#include "../h/riscv.hpp"
#include "../h/kernel.hpp"
#include "../h/syscall_c.hpp"
#include "../lib/console.h"
#include "../h/syscall_cpp.hpp"
#include "../h/print.hpp"
#include "../h/tcb.hpp"

extern void userMain();
extern "C" void table();

void um(void* arg){
    userMain();
    printStr("UserMain finished!\n");
}

void main(){

    mem_init();

    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
    __asm__ volatile("csrw stvec, %[stvecVal]": : [stvecVal] "r" (&table));
    __asm__ volatile("csrs stvec, 0x01");   // vektorski rezim

    TCB* mainThread = TCB::createThread(nullptr, nullptr, nullptr);

    TCB::running = mainThread;
    switchToUserMode();

    TCB* userMainThread = TCB::createThread(um, nullptr, mem_alloc(DEFAULT_STACK_SIZE));
    thread_dispatch();
    thread_join(userMainThread);

    printStr("Main continue!\n");

    delete userMainThread;
    delete mainThread;


    printStr("Finished\n");

    return;
}