#pragma once

#include "riscv.h"
#include "vsrtl_component.h"

namespace vsrtl {
using namespace core;

namespace RISCV {
class Branch : public Component {
public:
    Branch(std::string name, SimComponent* parent) : Component(name, parent) {
        // clang-format off
        res << [=] {
            switch(comp_op.uValue()){
                case CompOp::NOP: return false;
                case CompOp::EQ: return op1.uValue() == op2.uValue();
                case CompOp::NE: return op1.uValue() != op2.uValue();
                case CompOp::LT: return op1.sValue() < op2.sValue();
                case CompOp::LTU: return op1.uValue() < op2.uValue();
                case CompOp::GE: return op1.sValue() >= op2.sValue();
                case CompOp::GEU: return op1.uValue() >= op2.uValue();
                default: assert("Comparator: Unknown comparison operator"); return false;
            }
        };
        // clang-format on
    }

    INPUTPORT_ENUM(comp_op, CompOp);
    INPUTPORT(op1, RV_REG_WIDTH);
    INPUTPORT(op2, RV_REG_WIDTH);
    OUTPUTPORT(res, 1);
};

}  // namespace RISCV
}  // namespace vsrtl
