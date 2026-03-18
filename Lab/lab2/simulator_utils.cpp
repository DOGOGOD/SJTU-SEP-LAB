#include "simulator.h"

#include <iostream>
#include <format>

void Simulator::load_binary(const char *filename) {
    memory.load_binary(filename);
}

const Registers &Simulator::get_registers() const {
    return registers;
}

const Memory &Simulator::get_memory() const {
    return memory;
}

bool Simulator::error_invalid_reg(const uint8_t id) const {
    if (id >= Registers::REG_SIZE) {
        report_bad_reg(id);
        return true;
    }
    return false;
}

bool Simulator::error_valid_reg(const uint8_t id) const {
    if (id < Registers::REG_SIZE) {
        report_bad_reg(id);
        return true;
    }
    return false;
}

static const char *status_name(const Status status) {
    switch (status) {
        case Status::AOK: return "AOK";
        case Status::HLT: return "HLT";
        case Status::ADR: return "ADR";
        case Status::INS: return "INS";
    }
    return "Invalid Status";
}

void Simulator::report_stopped(const int step, const Status status) const {
    std::cout << std::format("Stopped in {} steps at PC = 0x{:x}.  Status '{}', CC {}\n",
                             step, pc, status_name(status), cc.name());
}

void Simulator::report_bad_inst_addr() const {
    std::cout << std::format("PC = 0x{:x}, Invalid instruction address\n", pc);
}

void Simulator::report_bad_data_addr(const uint64_t addr) const {
    std::cout << std::format("PC = 0x{:x}, Invalid data address 0x{:x}\n", pc, addr);
}

void Simulator::report_bad_stack_addr(const uint64_t sp) const {
    std::cout << std::format("PC = 0x{:x}, Invalid stack address 0x{:x}\n", pc, sp);
}

void Simulator::report_bad_reg(const uint8_t id) const {
    std::cout << std::format("PC = 0x{:x}, Invalid register ID 0x{:x}\n", pc, id);
}

void Simulator::report_bad_inst(const uint8_t inst) const {
    std::cout << std::format("PC = 0x{:x}, Invalid instruction {:02x}\n", pc, inst);
}
