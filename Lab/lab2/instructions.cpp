#include "instructions.h"

#include <stdexcept>

ConditionCodes::ConditionCodes()
    : ConditionCodes(true, false, false) {
}

ConditionCodes::ConditionCodes(const bool zf, const bool sf, const bool of)
    : value((zf << 2) | (sf << 1) | (of << 0)) {
}

static bool is_overflow(const AluOp op, const int64_t a, const int64_t b, const int64_t val) {
    switch (op) {
        case AluOp::ADD:
            return ((a < 0) == (b < 0)) && ((val < 0) != (a < 0));
        case AluOp::SUB:
            return ((a > 0) == (b < 0)) && ((val < 0) != (b < 0));
        default:
            return false;
    }
}

ConditionCodes ConditionCodes::compute(const AluOp op, const uint64_t a, const uint64_t b, const uint64_t val) {
    bool zero = (val == 0);
    bool sign = (static_cast<int64_t>(val) < 0);
    bool overflow = is_overflow(op, static_cast<int64_t>(a), static_cast<int64_t>(b), static_cast<int64_t>(val));

    return {zero, sign, overflow};
}

bool ConditionCodes::satisfy(const Condition cond) const {
    const bool zf = (value >> 2) & 1;
    const bool sf = (value >> 1) & 1;
    const bool of = (value >> 0) & 1;

    switch (cond) {
        case Condition::YES:
            return true;
        case Condition::LE:
            return (sf ^ of) | zf;
        case Condition::L:
            return sf ^ of;
        case Condition::E:
            return zf;
        case Condition::NE:
            return zf ^ 1;
        case Condition::GE:
            return sf ^ of ^ 1;
        case Condition::G:
            return (sf ^ of ^ 1) & (zf ^ 1);
    }
    return false;
}

const char *ConditionCodes::name() const {
    switch (value) {
        case 0: return "Z=0 S=0 O=0";
        case 1: return "Z=0 S=0 O=1";
        case 2: return "Z=0 S=1 O=0";
        case 3: return "Z=0 S=1 O=1";
        case 4: return "Z=1 S=0 O=0";
        case 5: return "Z=1 S=0 O=1";
        case 6: return "Z=1 S=1 O=0";
        case 7: return "Z=1 S=1 O=1";
        default: return "???????????";
    }
}
