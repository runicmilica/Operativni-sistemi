
#include "../h/kernel.hpp"
#include "../h/tcb.hpp"
#include "../h/scheduler.hpp"
#include "../h/_sem.hpp"

static uint64 NUM_OF_BLOCKS;
static void* blocks;
static uint8* two_bitVector;
static const uint8 FREE = 0x00;
static const uint8 USED = 0x01;
static const uint8 END = 0x02;

void mem_init(){
    size_t totalMem = (uint8*)HEAP_END_ADDR - (uint8*)HEAP_START_ADDR;
    NUM_OF_BLOCKS = totalMem / (MEM_BLOCK_SIZE + sizeof(uint8));
    two_bitVector = (uint8*)HEAP_START_ADDR;
    blocks = (uint8*)HEAP_START_ADDR + sizeof(uint8)*NUM_OF_BLOCKS;
    for(uint64 i = 0; i < NUM_OF_BLOCKS; i++) two_bitVector[i] = FREE;
    return;
}

void* mem_alloc_k (size_t size){
    // size je broj blokova koji treba alocirati
    if(size > NUM_OF_BLOCKS || size <= 0) return nullptr;
    uint64 cnt = 0;
    for(uint64 i = 0; i < NUM_OF_BLOCKS; i++){
        if(two_bitVector[i] == FREE) cnt++;
        if(two_bitVector[i] != FREE) cnt = 0;
        if(cnt == size){
            uint64 j = i - cnt + 1;
            for(; j < i; j++){
                two_bitVector[j] = USED;
            }
            two_bitVector[i] = END;
            return ((uint8*) blocks + (i - cnt + 1)*MEM_BLOCK_SIZE);
        }
    }
    return nullptr;
}

int mem_free_k (void* addr){

    uint64 index = ((uint8*)addr - (uint8*)blocks)/MEM_BLOCK_SIZE;

    if(index < 0 || index >= NUM_OF_BLOCKS) return -1;
    if((uint8*)addr < (uint8*)blocks ||
        (uint8*)addr > ((uint8*)HEAP_END_ADDR - MEM_BLOCK_SIZE)) return -2;

    for(uint32 i = index;;i++){
        if(two_bitVector[i] == FREE) return -3;
        if(two_bitVector[i] == USED) {
            two_bitVector[i]  = FREE;
            continue;
        }
        if(two_bitVector[i]  == END) {
            two_bitVector[i] = FREE;
            break;
        }
    }
    return 0;
}

int thread_create_k(thread_t* handle, void (*start_routine)(void*), void* arg, void* stack){
    // thread_t => TCB* => handle = TCB**
    *handle = TCB::createThread(start_routine, arg, stack);
    if(*handle != nullptr) return 0;
    return -1;
}

int thread_exit_k(){
    TCB::running->setFinished(true);
    return 0;
}

void thread_dispatch_k(){
    // TCB::dispatch();
}

void thread_join_k(thread_t handle){
    while(handle->isFinished() == false)
        TCB::yield();
}

int sem_open_k(sem_t* handle, unsigned init){
    *handle = new _sem(init);
    if(*handle != nullptr) return 0;
    return -1;
}
int sem_close_k (sem_t handle){
    // odblokiraj sve niti koje cekaju na ovom semaforu
    if(handle == nullptr) return -1;
    TCB* t = handle->get();
    while(t){
        t->setRetValue(-1);
        Scheduler::put(t);
        t = handle->get();
    }
    delete handle;
    return 0;
}
int sem_wait_k (sem_t id){
    if(id == nullptr) return -2;
    return id->wait();
}
int sem_signal_k (sem_t id){
    if(id == nullptr) return -2;
    return id->signal();
}

void putc_k(char c){
    __putc(c);
}
char getc_k(){
    return __getc();
}