#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <cassert>
#include <functional>

#include "Bus.h"

class CPU
{
private:
    //========================
    // BUS
    //========================
    std::unique_ptr<Bus> bus = std::make_unique<Bus>();

    //========================
    // REGISTERS
    //========================
    uint8_t A = 0;
    uint8_t B = 0, C = 0;
    uint8_t D = 0, E = 0;
    uint8_t H = 0, L = 0;

    uint16_t SP = 0;
    uint16_t PC = 0;

    uint8_t opcode = 0;
    int cycles = 0;

    //========================
    // FLAGS
    //========================
    struct Fbits
    {
        uint8_t F = 0;

        static constexpr uint8_t Z = 0x80;
        static constexpr uint8_t N = 0x40;
        static constexpr uint8_t H = 0x20;
        static constexpr uint8_t C = 0x10;

        void reset() { F = 0; }

        void set_z(bool v = true) { if (v) F |= Z; }
        void set_n(bool v = true) { if (v) F |= N; }
        void set_h(bool v = true) { if (v) F |= H; }
        void set_c(bool v = true) { if (v) F |= C; }

        int z() const { return (F >> 7) & 1; }
        int n() const { return (F >> 6) & 1; }
        int h() const { return (F >> 5) & 1; }
        int c() const { return (F >> 4) & 1; }
    } F;


    //========================
    // Operand Type
    //========================
    enum class OperandType
    {
        A, B, C, D, E, H, L,
        BC, DE, HL, SP,
        HL_MEM,
        BC_MEM,
        DE_MEM,
        NONE
    };

    inline const char* OpToString(OperandType op)
    {
        switch (op)
        {
            case OperandType::A: return "A"; break;
            case OperandType::B: return "B"; break;
            case OperandType::C: return "C"; break;
            case OperandType::D: return "D"; break; 
            case OperandType::E: return "E"; break;
            case OperandType::H: return "H"; break; 
            case OperandType::L: return "L"; break;
            case OperandType::HL_MEM: return "HL_MEM"; break;
            case OperandType::BC_MEM: return "BC_MEM"; break;
            case OperandType::DE_MEM: return "DE_MEM"; break;
            default: "None";
        }
    };



    //========================
    // INSTRUCTION TABLE
    //========================
    struct INSTRUCTION
    {
        std::string name;
        void (CPU::* opcode)(OperandType dst, OperandType scr);
        OperandType dst = OperandType::NONE;
        OperandType scr = OperandType::NONE;
        int cycles = 0;
    };

    std::vector<INSTRUCTION> lookuptable;

    //========================
    // HELPERS
    //========================
    inline uint16_t HL() const
    {
        return (H << 8) | L;
    }

    inline void setHL(uint16_t value)
    {
        H = value >> 8;
        L = value;
    }

    inline uint16_t BC() const
    {
        return (B << 8) | C;
    }


    inline void setBC(uint16_t value)
    {
        B = value >> 8;
        C = value;
    }

    inline uint16_t DE() const
    {
        return (D << 8) | E;
    }


    inline void setDE(uint16_t value)
    {
        D = value >> 8;
        E = value;
    }

    inline uint8_t read8(uint16_t addr) const
    {
        return bus->read(addr);
    }

    inline void write8(uint16_t addr, uint8_t v) const
    {
        bus->write(addr, v);
    }

    inline uint8_t fetch8()
    {
        return read8(PC++);
    }

    inline uint16_t fetch16()
    {
        uint8_t lo = fetch8();
        uint8_t hi = fetch8();
        return (hi << 8) | lo;
    }

    uint8_t readOperand(OperandType op);
    void writeOperand8(OperandType op, uint8_t value);
    void writeOperand16(OperandType op, uint16_t value);
    
public:
    CPU();

    void decode();

private:
    //========================
    // ADD
    //========================

    //void ADD(OperandType& dst, OperandType& src);

    //========================
    // SUB
    //========================
    //void SUB(OperandType dst, OperandType src);


    //========================
    // LOAD
    //========================

    void LD8(OperandType dst, OperandType scr);

    void LD16(OperandType dst, OperandType scr);

    void LDAA16(OperandType dst, OperandType scr);

    void LDAC(OperandType dst, OperandType scr);

    void LDAn(OperandType dst, OperandType scr);

    void LDHL_p(OperandType dst, OperandType scr);

    void LDHL_m(OperandType dst, OperandType scr);

    //========================
    // STACK
    //========================
    /*void PUSH_BC();
    void PUSH_DE();
    void PUSH_HL();
    void PUSH_AF();

    void POP_BC();
    void POP_DE();
    void POP_HL();
    void POP_AF();*/

    //========================
    // INTERNAL RESET
    //========================
    //void Reset();
};