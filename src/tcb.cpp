#include "../h/tcb.hpp"
#include "../h/riscv.hpp"

TCB *TCB::running = nullptr;

TCB *TCB::createThread(TCB::Body body, void *arg, void *stack)
{
    return new TCB(body, arg, (uint64 *)stack);
}

void TCB::yield()
{
    // Riscv::pushRegisters();

    TCB::dispatch();

    // Riscv::popRegisters();
}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished() && !old->isBlocked()) { Scheduler::put(old); }
    running = Scheduler::get();

    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::wrapper(void *arg)
{
    Riscv::popSppSpie();
    running->body(running->arg);
    // running->setFinished(true);
    thread_exit();
    // TCB::yield();
    thread_dispatch();
}

void TCB::runWrapper(void *arg) {
    ((Thread*)arg)->run();
}