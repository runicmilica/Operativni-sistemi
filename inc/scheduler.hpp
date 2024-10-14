#ifndef PROJECT_SCHEDULER_HPP
#define PROJECT_SCHEDULER_HPP

#include "list.hpp"

class TCB;

class Scheduler
{
private:
    static List<TCB> readyCoroutineQueue;

public:
    static TCB *get();

    static void put(TCB *tcb);

};

#endif