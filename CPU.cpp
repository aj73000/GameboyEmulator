#include "CPU.h"

//=========================
// CONSTRUCTOR
//========================
CPU::CPU()
{
    lookuptable.resize(256);

    lookuptable[0x06] = { "LD B,d8", &CPU::LD8, OperandType::B, OperandType::NONE , 8 };

}

//=========================
// Read Operand
//========================
uint8_t CPU::readOperand(OperandType op)
{
    switch (op)
    {
    case OperandType::A: return A;
    case OperandType::B: return B;
    case OperandType::C: return C;
    case OperandType::D: return D;
    case OperandType::E: return E;
    case OperandType::H: return H;
    case OperandType::L: return L;

    case OperandType::HL_MEM:
        return read8(HL());

    case OperandType::BC_MEM:
        return read8(BC());

    case OperandType::DE_MEM:
        return read8(DE());

    case OperandType::NONE:
        throw std::runtime_error("Tried to read NONE as an operand in readOperand");

    default:
        throw std::runtime_error("Invalid operand");
    }
}

//========================
// Write Operand
//========================
void CPU::writeOperand8(OperandType op, uint8_t value)
{
    switch (op)
    {
    case OperandType::A: A = value; break;
    case OperandType::B: B = value; break;
    case OperandType::C: C = value; break;
    case OperandType::D: D = value; break;
    case OperandType::E: E = value; break;
    case OperandType::H: H = value; break;
    case OperandType::L: L = value; break;

    case OperandType::HL_MEM:
        write8(HL(), value);
        break;

    case OperandType::BC_MEM:
        write8(BC(), value);
        break;

    case OperandType::DE_MEM:
        write8(DE(), value);
        break;

    default:
        break;
    }
}

void CPU::writeOperand16(OperandType op, uint16_t value)
{
    switch (op)
    {
    case OperandType::HL:
        setHL(value);
        break;

    case OperandType::BC:
        setBC(value);
        break;

    case OperandType::DE:
        setDE(value);
        break;

    default:
        // Not ideal and will truncate if called
        writeOperand8(op, static_cast<uint8_t>(value));
        std::cout << "Truncation under writeOperand16\n";
        break;
    }
}

//=========================
// DECODE LOOP
//========================
void CPU::decode()
{
    if (cycles == 0)
    {
        opcode = fetch8();

        auto& ins = lookuptable[opcode];

        if (!ins.opcode)
            return;

        (this->*ins.opcode)(ins.dst, ins.scr);

        cycles = ins.cycles;
    }
    cycles--;
}

//=========================
// LOAD
//========================
void CPU::LD8(OperandType dst, OperandType scr)
{
    uint8_t value = 0;
    if (scr == OperandType::NONE)
        value = fetch8();
    else
        value = readOperand(scr);

    writeOperand8(dst, value);

    //Debug
    std::cout << "LD8: Location: " << OpToString(dst) << " Value: " << readOperand(dst) << "\n";
};

void CPU::LD16(OperandType dst, OperandType scr)
{
    uint16_t value = 0;
    if (scr == OperandType::NONE)
    {
        // LD rr, nn: Load 16-bit register / register pair
        value = fetch16();
        writeOperand16(dst, value);
        return;
    }
    else if (dst == OperandType::NONE && scr == OperandType::SP)
    {
        //Do LD (nn), SP: Load from stack pointer (direct)


        return;
    }
    else if (dst == OperandType::SP && scr == OperandType::HL)
    {
        //LD SP, HL: Load stack pointer from HL


        return;
    }
    else if (dst == OperandType::HL && scr == OperandType::SP)
    {
        //LD HL, SP+e: Load HL from adjusted stack pointer


        return;
    }
    throw std::runtime_error("Invalid use of LD16 (scr should be NONE)\n");
};

void CPU::LDAA16(OperandType dst, const OperandType scr = OperandType::NONE)
{
    // if dst is A then we are storing the 16 bit immediate address in A 
    // else its the value of A in the address of the location given by the 16 bit address
    uint16_t addr = fetch16();
    dst == OperandType::A ? writeOperand8(OperandType::A, read8(addr)) : write8(addr, readOperand(OperandType::A));
};


void CPU::LDAC(OperandType dst, OperandType scr)
{
    uint16_t addr = 0xFF00 + C;
    dst == OperandType::A ? writeOperand8(OperandType::A, read8(addr)) : write8(addr, readOperand(OperandType::A));
};

void CPU::LDAn(OperandType dst, OperandType scr)
{
    uint16_t addr = 0xFF00 + fetch8();
    dst == OperandType::A ? writeOperand8(OperandType::A, read8(addr)) : write8(addr, readOperand(OperandType::A));
};


void CPU::LDHL_p(OperandType dst, OperandType scr)
{
    uint16_t addr = HL();
    dst == OperandType::A ? writeOperand8(OperandType::A, read8(addr)) : write8(addr, readOperand(OperandType::A));
    writeOperand16(OperandType::HL, addr + 1);
};

void CPU::LDHL_m(OperandType dst, OperandType scr)
{
    uint16_t addr = HL();
    dst == OperandType::A ? writeOperand8(OperandType::A, read8(addr)) : write8(addr, readOperand(OperandType::A));
    writeOperand16(OperandType::HL, addr - 1);
};