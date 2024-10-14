#include "../h/_sem.hpp"

void _sem::put(TCB* t){
   blocked.addLast(t);
}
TCB* _sem::get(){
    return blocked.removeFirst();
}
int _sem::wait(){
    val--;
    if(val < 0) {
        // blokiraj nit i promeni kontekst
        TCB* old = TCB::running;
        // stavi nit u red blokiranih
        put(old);
        TCB::running->setBlocked(true);
        TCB::dispatch();
    }
    return TCB::running->getRetValue();
}
int _sem::signal(){
    val++;
    if(val <= 0){
        // odblokiraj nit
        // uzmi jednu nit iz reda blokiranih
        TCB * t = get();
        t->setBlocked(false);
        if(t != nullptr) Scheduler::put(t);
    }
    return 0;
}