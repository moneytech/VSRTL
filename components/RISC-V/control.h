#pragma once

#include "riscv.h"
#include "vsrtl_component.h"

namespace vsrtl {
using namespace core;

namespace RISCV {

class Control : public Component {
public:
    Control(std::string name, SimComponent* parent) : Component(name, parent) {
        // clang-format off
        comp_ctrl << [=] {
            switch(opcode.uValue()){
                case RVInstr::BEQ: return CompOp::EQ;
                case RVInstr::BNE: return CompOp::NE;
                case RVInstr::BLT: return CompOp::LT;
                case RVInstr::BGE: return CompOp::GE;
                case RVInstr::BLTU: return CompOp::LTU;
                case RVInstr::BGEU: return CompOp::GEU;
                default: return CompOp::NOP;
            }
        };

        do_branch << [=] {
            switch(opcode.uValue()){
                case RVInstr::BEQ: case RVInstr::BNE: case RVInstr::BLT:
                case RVInstr::BGE: case RVInstr::BLTU: case RVInstr::BGEU:
                    return 1;
                default:
                    return 0;
            }
        };

        do_jump << [=] {
            switch(opcode.uValue()){
                case RVInstr::JAL: case RVInstr::JALR:
                    return 1;
                default:
                    return 0;
            }
        };

        mem_ctrl << [=] {
            switch(opcode.uValue()){
                case RVInstr::SB: return MemOp::SB;
                case RVInstr::SH: return MemOp::SH;
                case RVInstr::SW: return MemOp::SW;
                case RVInstr::LB: return MemOp::LB;
                case RVInstr::LH: return MemOp::LH;
                case RVInstr::LW: return MemOp::LW;
                case RVInstr::LBU: return MemOp::LBU;
                case RVInstr::LHU: return MemOp::LHU;
                default:
                    return MemOp::NOP;
            }
        };

        reg_do_write_ctrl << [=] {
            switch(opcode.uValue()) {
                case RVInstr::LUI:
                case RVInstr::AUIPC:

                // Arithmetic-immediate instructions
                case RVInstr::ADDI: case RVInstr::SLTI: case RVInstr::SLTIU: case RVInstr::XORI:
                case RVInstr::ORI: case RVInstr::ANDI: case RVInstr::SLLI: case RVInstr::SRLI:
                case RVInstr::SRAI:

                // Arithmetic instructions
                case RVInstr::MUL: case RVInstr::MULH: case RVInstr:: MULHSU: case RVInstr::MULHU:
                case RVInstr::DIV: case RVInstr::DIVU: case RVInstr::REM: case RVInstr::REMU:
                case RVInstr::ADD: case RVInstr::SUB: case RVInstr::SLL: case RVInstr::SLT:
                case RVInstr::SLTU: case RVInstr::XOR: case RVInstr::SRL: case RVInstr::SRA:
                case RVInstr::OR: case RVInstr::AND:

                // Load instructions
                case RVInstr::LB: case RVInstr::LH: case RVInstr::LW: case RVInstr::LBU: case RVInstr::LHU:

                // Jump instructions
                case RVInstr::JALR:
                case RVInstr::JAL:
                    return 1;
                default: return 0;
            }
        };

        reg_wr_src_ctrl << [=] {
            switch(opcode.uValue()){
                // Load instructions
                case RVInstr::LB: case RVInstr::LH: case RVInstr::LW: case RVInstr::LBU: case RVInstr::LHU:
                    return RegWrSrc::MEMREAD;

                // Jump instructions
                case RVInstr::JALR:
                case RVInstr::JAL:
                    return RegWrSrc::PC4;

                default:
                    return RegWrSrc::ALURES;
            }
        };

        alu_op1_ctrl << [=] {
            switch(opcode.uValue()) {
                case RVInstr::AUIPC: case RVInstr::JAL:
                case RVInstr::BEQ: case RVInstr::BNE: case RVInstr::BLT:
                case RVInstr::BGE: case RVInstr::BLTU: case RVInstr::BGEU:
                    return AluSrc1::PC;
                default:
                    return AluSrc1::REG1;
            }
        };

        alu_op2_ctrl << [=] {
            switch(opcode.uValue()) {
            case RVInstr::LUI:
            case RVInstr::AUIPC:
                return AluSrc2::IMM;

            // Arithmetic-immediate instructions
            case RVInstr::ADDI: case RVInstr::SLTI: case RVInstr::SLTIU: case RVInstr::XORI:
            case RVInstr::ORI: case RVInstr::ANDI: case RVInstr::SLLI: case RVInstr::SRLI:
            case RVInstr::SRAI:
                return AluSrc2::IMM;

            // Arithmetic instructions
            case RVInstr::MUL: case RVInstr::MULH: case RVInstr:: MULHSU: case RVInstr::MULHU:
            case RVInstr::DIV: case RVInstr::DIVU: case RVInstr::REM: case RVInstr::REMU:
            case RVInstr::ADD: case RVInstr::SUB: case RVInstr::SLL: case RVInstr::SLT:
            case RVInstr::SLTU: case RVInstr::XOR: case RVInstr::SRL: case RVInstr::SRA:
            case RVInstr::OR: case RVInstr::AND:
                return AluSrc2::REG2;

            // Load instructions
            case RVInstr::LB: case RVInstr::LH: case RVInstr::LW: case RVInstr::LBU: case RVInstr::LHU:
                return AluSrc2::IMM;

            // Store instructions
            case RVInstr::SB: case RVInstr::SH: case RVInstr::SW:
                return AluSrc2::REG2;

            // Branch instructions
            case RVInstr::BEQ: case RVInstr::BNE: case RVInstr::BLT:
            case RVInstr::BGE: case RVInstr::BLTU: case RVInstr::BGEU:
                return AluSrc2::IMM;

            // Jump instructions
            case RVInstr::JALR:
            case RVInstr::JAL:
                return AluSrc2::IMM;

            default:
                return AluSrc2::REG2;
            }
        };

        mem_do_read_ctrl << [=] {
            switch(opcode.uValue()) {
                case RVInstr::LB: case RVInstr::LH: case RVInstr::LW: case RVInstr::LBU: case RVInstr::LHU:
                    return 1;
                default: return 0;
            }
        };

        pc_src_ctrl << [=] {
            switch(opcode.uValue()){
            case RVInstr::BEQ: case RVInstr::BNE: case RVInstr::BLT:
            case RVInstr::BGE: case RVInstr::BLTU: case RVInstr::BGEU:
            case RVInstr::JALR:
            case RVInstr::JAL:
                return PcSrc::ALU;
            default:
                return PcSrc::PC4;
            }
        };

        alu_ctrl << [=] {
            switch(opcode.uValue()) {
                case RVInstr::LUI:
                    return ALUOp::LUI;
                case RVInstr::JAL: case RVInstr::JALR: case RVInstr::AUIPC:
                case RVInstr::ADD: case RVInstr::ADDI:
                case RVInstr::BEQ: case RVInstr::BNE: case RVInstr::BLT:
                case RVInstr::BGE: case RVInstr::BLTU: case RVInstr::BGEU:
                    return ALUOp::ADD;
                case RVInstr::SUB:
                    return ALUOp::SUB;
                case RVInstr::SLT: case RVInstr::SLTI:
                    return ALUOp::LT;
                case RVInstr::SLTU: case RVInstr::SLTIU:
                    return ALUOp::LTU;
                case RVInstr::XOR: case RVInstr::XORI:
                    return ALUOp::XOR;
                case RVInstr::OR: case RVInstr::ORI:
                    return ALUOp::OR;
                case RVInstr::AND: case RVInstr::ANDI:
                    return ALUOp::AND;
                case RVInstr::SLL: case RVInstr::SLLI:
                    return ALUOp::SL;
                case RVInstr::SRL: case RVInstr::SRLI:
                    return ALUOp::SRL;
                case RVInstr::SRA: case RVInstr::SRAI:
                    return ALUOp::SRA;
                case RVInstr::MUL:
                    return ALUOp::MUL;
                case RVInstr::MULH:
                    return ALUOp::MULH;
                case RVInstr::MULHU:
                    return ALUOp::MULHU;
                case RVInstr::MULHSU:
                    return ALUOp::MULHSU;
                case RVInstr::DIV:
                    return ALUOp::DIV;
                case RVInstr::DIVU:
                    return ALUOp::DIVU;
                case RVInstr::REM:
                    return ALUOp::REM;
                case RVInstr::REMU:
                    return ALUOp::REMU;
                default: return ALUOp::NOP;
            }
        };

        mem_do_write_ctrl << [=] {
            switch(opcode.uValue()) {
                case RVInstr::SB: case RVInstr::SH: case RVInstr::SW:
                    return 1;
                default: return 0;
            }
        };

        // clang-format on
    }

    INPUTPORT_ENUM(opcode, RVInstr);

    OUTPUTPORT(reg_do_write_ctrl, 1);
    OUTPUTPORT(mem_do_read_ctrl, 1);
    OUTPUTPORT(mem_do_write_ctrl, 1);
    OUTPUTPORT(do_branch, 1);
    OUTPUTPORT(do_jump, 1);

    OUTPUTPORT_ENUM(comp_ctrl, CompOp);
    OUTPUTPORT_ENUM(reg_wr_src_ctrl, RegWrSrc);
    OUTPUTPORT_ENUM(mem_ctrl, MemOp);
    OUTPUTPORT_ENUM(pc_src_ctrl, PcSrc);

    OUTPUTPORT_ENUM(alu_op1_ctrl, AluSrc1);
    OUTPUTPORT_ENUM(alu_op2_ctrl, AluSrc2);
    OUTPUTPORT_ENUM(alu_ctrl, ALUOp);
};

}  // namespace RISCV
}  // namespace vsrtl
