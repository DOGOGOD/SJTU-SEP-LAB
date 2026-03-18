#include "registers.h"

#include <iostream>
#include <format>

Registers::Registers() : data() {
}

uint64_t &Registers::operator[](const uint8_t id) {
    return data.at(id);
}

uint64_t &Registers::operator[](RegId id) {
    return data[static_cast<size_t>(id)];
}

void Registers::diff(const Registers &r0, const Registers &r1) {
    for (int i = 0; i < REG_SIZE; i++) {
        if (uint64_t v0 = r0.data[i], v1 = r1.data[i]; v0 != v1) {
            std::cout << std::format("{}:\t0x{:016x}\t0x{:016x}\n",
                                     name(i), v0, v1);
        }
    }
}

const char *Registers::name(const int id) {
    switch (id) {
        case 0: return "%rax";
        case 1: return "%rcx";
        case 2: return "%rdx";
        case 3: return "%rbx";
        case 4: return "%rsp";
        case 5: return "%rbp";
        case 6: return "%rsi";
        case 7: return "%rdi";
        case 8: return "%r8";
        case 9: return "%r9";
        case 10: return "%r10";
        case 11: return "%r11";
        case 12: return "%r12";
        case 13: return "%r13";
        case 14: return "%r14";
        default: return "----";
    }
}
