#ifndef PROJECT_TCB_HPP
#define PROJECT_TCB_HPP

#include "kernel.hpp"
#include "scheduler.hpp"
// Thread control block
class TCB{

public:
    void* operator new (size_t size){ return mem_alloc_k((size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);}
    void operator delete (void* addr){ mem_free_k(addr);}
    void* operator new[](uint64 size){ return mem_alloc_k((size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);}
    void operator delete[](void* addr){ mem_free_k(addr);}

    using Body = void (*)(void*);

    bool isFinished() const { return finished; }
    int getRetValue() const { return retValue; }
    bool isBlocked() const { return blocked; }
    void setRetValue(int val) { retValue = val; }
    void setFinished(bool value) { finished = value; }
    void setBlocked(bool value) { blocked = value; }

    ~TCB() { delete [] stack; }
    static TCB* createThread(Body body, void *arg, void* stack);
    static TCB* running;
    static void yield();
    static void runWrapper(void* arg);

private:

    TCB(Body body, void *arg, uint64 * stack):
        body(body),
        arg(arg),
        stack(body != nullptr ? (uint64 *) stack:nullptr),
        context({body != nullptr ? (uint64) &wrapper : 0,
                 stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0}),
        finished(false), blocked(false), retValue(0)
    {
        if(body)
            Scheduler::put(this);
    }
    struct Context {
        uint64 ra;
        uint64 sp;
    };
    Body body;
    void* arg;
    uint64 *stack;
    Context context;
    bool finished;
    bool blocked;
    int retValue;

    friend class Riscv;
    friend class _sem;

    static void wrapper(void *arg);
    static void contextSwitch(Context *oldContext, Context *runningContext);
    static void dispatch();
};

#endif