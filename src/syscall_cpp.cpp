#include "../h/syscall_cpp.hpp"
#include "../h/tcb.hpp"
#include "../h/_sem.hpp"

void* operator new (size_t size){ return mem_alloc(size);}
void operator delete (void* addr){ mem_free(addr);}
void* operator new[](uint64 size){ return mem_alloc(size);}
void operator delete[](void* addr){ mem_free(addr);}
// Thread
Thread::Thread (void (*body)(void*), void* arg){
    // kreira nit i TCB, ali ga ne stavlja u scheduler
    // u ra je adresa wrappera koji ce da se zove pri pokretanju niti prvi put
    // koji se prvo vrati sa sret iz prekidne rutine i zove body da se izvrsava
    // kada se body zavrsi stavlja u finished true i zove yield?
    Thread::myHandle = nullptr;
    Thread::body = body;
    Thread::arg = arg;
    // thread_create(&myHandle, body, arg);
}
Thread::~Thread (){
    // delete ce da zove destruktor za myHandle koji brise stack
    // onda obrise i myHandle
    delete myHandle;
}
int Thread::start (){
    if(!myHandle){
        // thread_create stavlja nit u Scheduler
        thread_create(&myHandle, body, arg);
        return 0;
    }
    // Scheduler::put(myHandle);
    // nit je vec startovana
    return -1;
}
void Thread::join(){
    thread_join(this->myHandle);
}
void Thread::dispatch (){
    thread_dispatch();
}
int Thread::sleep (time_t){
    return 0;
}
// koja je poenta ovog konstruktora?
Thread::Thread (){
    myHandle = nullptr;
    Thread::body = &TCB::runWrapper;
    Thread::arg = this;
    // thread_create(&myHandle, &TCB::runWrapper, this);
}
// Semaphore
Semaphore::Semaphore (unsigned init){
    myHandle = nullptr;
    sem_open(&myHandle, init);
}
Semaphore::~Semaphore (){
    sem_close(myHandle);
}
int Semaphore::wait (){
    return sem_wait(myHandle);
}
int Semaphore::signal (){
    return sem_signal(myHandle);
}

void Console::putc(char c){
    ::putc(c);
}

char Console::getc(){
    return ::getc();
}