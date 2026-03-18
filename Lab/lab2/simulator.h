#pragma once

#include <cstdint>

#include "memory.h"
#include "registers.h"
#include "instructions.h"

enum class Status { AOK, HLT, ADR, INS };

class Simulator {
    uint64_t pc;
    Registers registers;
    Memory memory;
    ConditionCodes cc;

public:
    Simulator();

    /** 加载二进制文件到 memory 中。 */
    void load_binary(const char *filename);

    [[nodiscard]]
    const Registers &get_registers() const;

    [[nodiscard]]
    const Memory &get_memory() const;

    /**
     * 开始执行指令。遇到异常状态码时停机，并输出处理器状态。
     * @param max_steps 最多执行多少步。
     */
    void run(int max_steps);

private:
    /** 执行下一条指令，返回状态码。 */
    Status next_instruction();

    /**
     * 寄存器 ID 无效时，输出错误。用来检查有效的 ID。
     * @return true 表示寄存器无效，此时应该返回 Status::INS 状态码。
     */
    [[nodiscard]]
    bool error_invalid_reg(uint8_t id) const;

    /**
     * 寄存器 ID 有效时，输出错误。用来检查应该为空的 ID (0xF)。
     * @return true 表示寄存器有效，此时应该返回 Status::INS 状态码。
     */
    [[nodiscard]]
    bool error_valid_reg(uint8_t id) const;

    /** 处理器停机后，输出当前状态。 */
    void report_stopped(int step, Status status) const;

    // 以下方法用来输出评测要求的报错信息。

    /** PC 指向的地址无效，无法正确读取指令。 */
    void report_bad_inst_addr() const;

    /** 访存指令(rmmovq,mrmovq)访问的内存地址无效。 */
    void report_bad_data_addr(uint64_t addr) const;

    /** 压栈、弹栈(call,ret,pushq,popq)时，%rsp 指向的内存地址无效。 */
    void report_bad_stack_addr(uint64_t sp) const;

    /**
     * 指令中的寄存器 ID 不符合指令集的要求。
     * 使用 error_invalid_reg 或者 error_valid_reg 时，不需要再次调用。
     */
    void report_bad_reg(uint8_t id) const;

    /** 无法解读指令。 */
    void report_bad_inst(uint8_t inst) const;
};
