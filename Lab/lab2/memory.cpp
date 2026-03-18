#include "memory.h"

#include <format>
#include <fstream>
#include <iostream>
#include <stdexcept>

Memory::Memory() : data() {
}

void Memory::load_binary(const char *filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Can't open binary file");
    }
    file.read(reinterpret_cast<char *>(data.data()), data.size());
    if (file.bad()) {
        throw std::runtime_error("Failed to load binary file");
    }
    if (!file.eof()) {
        throw std::runtime_error("too large memory footprint");
    }
}

std::optional<uint8_t> Memory::get_byte(const uint64_t addr) const {
    if (addr >= data.size()) {
        return std::nullopt;
    }
    return data[addr];
}

bool Memory::set_byte(const uint64_t addr, const uint8_t value) {
    if (addr >= data.size()) {
        return false;
    }
    data[addr] = value;
    return true;
}

std::optional<uint64_t> Memory::get_long(const uint64_t addr) const {
    if (addr >= data.size() || addr + 8 >= data.size()) {
        return std::nullopt;
    }
    uint64_t result = 0;
    for (int i = 0; i < 8; i++) {
        result |= static_cast<uint64_t>(data[addr + i]) << (8 * i);
    }
    return result;
}

bool Memory::set_long(const uint64_t addr, uint64_t value) {
    if (addr >= data.size() || addr + 8 >= data.size()) {
        return false;
    }
    for (int i = 0; i < 8; i++) {
        data[addr + i] = value & 0xFF;
        value >>= 8;
    }
    return true;
}

void Memory::diff(const Memory &m0, const Memory &m1) {
    for (uint64_t addr = 0; addr < m0.data.size(); addr += 8) {
        if (uint64_t v0 = *m0.get_long(addr), v1 = *m1.get_long(addr); v0 != v1) {
            std::cout << std::format("0x{:016x}:\t0x{:016x}\t0x{:016x}\n",
                                     addr, v0, v1);
        }
    }
}
