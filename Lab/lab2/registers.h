#pragma once

#include <cstdint>
#include <array>

enum class RegId : uint8_t {
    RAX = 0,
    RCX = 1,
    RDX = 2,
    RBX = 3,
    RSP = 4,
    RBP = 5,
    RSI = 6,
    RDI = 7,
    R8 = 8,
    R9 = 9,
    R10 = 10,
    R11 = 11,
    R12 = 12,
    R13 = 13,
    R14 = 14,
};

class Registers {
public:
    Registers();

    /**
     * 重载了 Registers 类的下标运算符，可以使用方括号来读写寄存器。例如：
     * @code
     * uint8_t reg_a, reg_b;
     * std::cout << registers[reg_a];
     * registers[reg_b] = registers[reg_a];
     * registers[reg_a] = 0x123;
     * @endcode
     */
    uint64_t &operator[](uint8_t id);

    /**
     * 重载了 Registers 类的下标运算符，可以使用方括号来读写寄存器。例如：
     * @code
     * uint64_t sp = registers[RegId::RSP] - 8;
     * registers[RegId::RSP] = sp;
     * @endcode
     */
    uint64_t &operator[](RegId id);

    /** 输出寄存器的变动。 */
    static void diff(const Registers &r0, const Registers &r1);

    /** 寄存器的名称。 */
    static const char *name(int id);

    static constexpr uint8_t REG_SIZE = 15;

private:
    std::array<uint64_t, REG_SIZE> data;
};
