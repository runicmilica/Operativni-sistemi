#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../h/print.hpp"
#include "../lib/console.h"


void Riscv::popSppSpie()
{
    mc_sstatus(SSTATUS_SPP);
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleIntr(uint64 a0, uint64 a1, uint64 a2, uint64 a3, uint64 a4) {

    uint64 scauseVar = Riscv::r_scause();

    if(scauseVar == 0x08 || scauseVar == 0x09){
        // sistemski poziv preko ecall
        uint64 volatile sepc = Riscv::r_sepc() + 4; // jer pc pokazuje na trenutnu instrukciju tj ecall a treba na sledecu
        uint64 volatile sstatus = Riscv::r_sstatus(); // cuva vrednost statusnog registra

        if (a0 == 0x01) {
            mem_alloc_k((uint64) a1);
        } else if (a0 == 0x02) {
            mem_free_k((void *) a1);
        } else if (a0 == 0x03) {
            Riscv::w_sepc(sepc);
            Riscv::w_sstatus(sstatus);
            Riscv::mc_sstatus(SSTATUS_SPP);
            return;
        } else if (a0 == 0x11) {
            thread_create_k((thread_t *) a1, (void (*)(void *)) a2, (void *) a3, (void *) a4);
        } else if (a0 == 0x12) {
            thread_exit_k();
        } else if (a0 == 0x13) {
            TCB::dispatch();
        } else if (a0 == 0x14) {
            thread_join_k((thread_t) a1);
        } else if (a0 == 0x21) {
            sem_open_k((sem_t *) a1, (unsigned) a2);
        } else if (a0 == 0x22) {
            sem_close_k((sem_t) a1);
        } else if (a0 == 0x23) {
            sem_wait_k((sem_t) a1);
        } else if (a0 == 0x24) {
            sem_signal_k((sem_t) a1);
        } else if (a0 == 0x41) {
            getc_k();
        } else if (a0 == 0x42) {
            putc_k((char) a1);
        }

        __asm__ volatile ("sd a0, 10*8(s0)");
        Riscv::w_sepc(sepc);
        Riscv::w_sstatus(sstatus);
        return;
    }
    else{
        // uint64 volatile sepc = Riscv::r_sepc() + 4; // jer pc pokazuje na trenutnu instrukciju tj ecall a treba na sledecu
        // uint64 volatile sstatus = Riscv::r_sstatus(); // cuva vrednost statusnog registra
        printStr("Scause: ");
        printInteger(scauseVar);
        printStr("\n");
        while(true){}
        // Riscv::w_sepc(sepc);
        // Riscv::w_sstatus(sstatus);
        // return;
    }
}

void Riscv::handleTime() {
    uint64 scauseVar = Riscv::r_scause();
    if (scauseVar == 0x8000000000000001UL)
    {
        __asm__ volatile ("csrc sip, 0x02");
    }
    return;
}

void Riscv::handleCons() {
    uint64 scauseVar = Riscv::r_scause();
    if (scauseVar == 0x8000000000000009UL)
    {
        console_handler();
        __asm__ volatile ("csrc sip, 0x02");
    }
    return;
}