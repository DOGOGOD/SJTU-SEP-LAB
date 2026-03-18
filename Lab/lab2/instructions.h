#pragma once

#include <cstdint>

enum class InstructionCode : uint8_t {
    HALT = 0x0,
    NOP = 0x1,
    RRMOVQ = 0x2,
    IRMOVQ = 0x3,
    RMMOVQ = 0x4,
    MRMOVQ = 0x5,
    ALU = 0x6,
    JMP = 0x7,
    CALL = 0x8,
    RET = 0x9,
    PUSHQ = 0xA,
    POPQ = 0xB,
};

enum class AluOp : uint8_t {
    ADD = 0x0,
    SUB = 0x1,
    AND = 0x2,
    XOR = 0x3,
};

enum class Condition : uint8_t {
    YES = 0x0,
    LE = 0x1,
    L = 0x2,
    E = 0x3,
    NE = 0x4,
    GE = 0x5,
    G = 0x6,
};

class ConditionCodes {
public:
    /** 初始值。 */
    ConditionCodes();

    /**
     * 计算 ALU 操作的 CC 值。
     * @param op 操作
     * @param a 第一个操作数
     * @param b 第二个操作数
     * @param val ALU 计算结果
     */
    [[nodiscard]]
    static ConditionCodes compute(AluOp op, uint64_t a, uint64_t b, uint64_t val);

    /** 是否满足跳转(jXX)/条件移动(cmovXX)的条件。 */
    [[nodiscard]]
    bool satisfy(Condition cond) const;

    [[nodiscard]]
    const char *name() const;

private:
    uint8_t value;

    /**
     * @param zf zero flag
     * @param sf sign flag
     * @param of overflow flag
     */
    ConditionCodes(bool zf, bool sf, bool of);
};
