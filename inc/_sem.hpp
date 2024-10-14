#ifndef PROJECT_SEM_HPP
#define PROJECT_SEM_HPP

#include "tcb.hpp"

class _sem{
public:
    void* operator new (size_t size){ return mem_alloc_k((size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);}
    void operator delete (void* addr){ mem_free_k(addr);}
    void* operator new[](uint64 size){ return mem_alloc_k((size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);}
    void operator delete[](void* addr){ mem_free_k(addr);}

    _sem(unsigned val):val((int)val){}
    void put(TCB* t);
    TCB* get();
    int wait();
    int signal();
private:
    int val;
    List<TCB> blocked;
    friend class Riscv;
};

#endif