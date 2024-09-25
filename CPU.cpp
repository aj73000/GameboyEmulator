#include "CPU.h"


CPU::CPU(){};

uint8_t CPU::read(uint16_t&& address)
{
	assert(address < bus->ram.size());
	std::cout << address << std::endl;
	return bus->read(address);
};

void CPU::write(uint16_t&& addr, uint8_t& data)
{
	bus->write(addr, data);
}

void CPU::decode()
{
	
	if (cycles == 0)
	{
		std::cout << "Address: ";
		this->opcode = read(PC++);

		std::cout << "opcode: " << (long)this->opcode << std::endl;

		//This will be Data:"Blank" if no data is fetched else data location will show i.e Data: "Location"
		std::cout << "DataLocation: ";

		// Use opcode on table
		(this->*(*lookuptable)[opcode].opcode)(*(*lookuptable)[opcode].value1, *(*lookuptable)[opcode].value2);

		cycles = (*lookuptable)[opcode].numCycles;
	}
	std::cout<< "Cycles: " << cycles << "\n";
	cycles--;
}

void CPU::reset()
{

}

void CPU::ADD(uint8_t& r1, uint8_t& r2)
{
	uint8_t result = r1 + r2, carry_per_bit = r1 + r2;
	r1 = result;

	this->F = 0;

	this->F += result == 0 ? 128 : 0;

	// if (carry_per_bit[3] then set H(ie add 32))
	this->F += (carry_per_bit >> 3) & 1 ? 32 : 0;
	this->F += (carry_per_bit >> 7) & 1 ? 16 : 0;
}

void CPU::ADDHL(uint8_t& r1, uint8_t& r2)
{
	uint8_t data = read((this->H << 4) + this->L);
	uint8_t result = r1 + data, carry_per_bit = r1 + data;

	r1 = result;

	this->F = 0;

	this->F += result == 0 ? 128 : 0;
	this->F += (carry_per_bit >> 3) & 1 ? 32 : 0;
	this->F += (carry_per_bit >> 7) & 1 ? 16 : 0;

}
void CPU::ADDI(uint8_t& r1, uint8_t& r2)
{
	uint8_t n = read(PC++);
	uint8_t result = r1 + n, carry_per_bit = r1 + n;

	r1 = result;

	this->F = 0;
	this->F += result == 0 ? 128 : 0;
	this->F += (carry_per_bit >> 3) & 1 ? 32 : 0;
	this->F += (carry_per_bit >> 7) & 1 ? 16 : 0;
}

void CPU::ADC(uint8_t& r1, uint8_t& r2)
{
	uint8_t result = this->A + r2 + (this->F & 0b00010000) , carry_per_bit = this->A + r2 + (this->F & 0b00010000);

	this->A = result;

	this->F = 0;
	this->F += result == 0 ? 128 : 0;
	this->F += (carry_per_bit >> 4) & 1 ? 32 : 0;
	this->F += carry_per_bit & 1 ? 16 : 0;
}

void CPU::ADCHL(uint8_t& r1, uint8_t& r2)
{
	uint8_t data = read((this->H << 4) + this->L);

	uint8_t result = this->A + data + (this->F & 0b00010000), carry_per_bit = this->A + data + (this->F & 0b00010000);

	this->A = result;

	this->F = 0;
	this->F += result == 0 ? 128 : 0;
	this->F += (carry_per_bit >> 3) & 1 ? 32 : 0;
	this->F += (carry_per_bit >> 7) & 1 ? 16 : 0;
}

void CPU::ADCI(uint8_t& r1, uint8_t& r2)
{
	uint8_t n = read(PC++);

	uint8_t result = this->A + n + (this->F & 0b00010000), carry_per_bit = this->A + n + (this->F & 0b00010000);

	this->A = result;

	this->F = 0;
	this->F += result == 0 ? 128 : 0;
	this->F += (carry_per_bit >> 3) & 1 ? 32 : 0;
	this->F += (carry_per_bit >> 7) & 1 ? 16 : 0;
}

void CPU::SUB(uint8_t& r1, uint8_t& r2)
{
	uint8_t result = this->A - r2, carry_per_bit = this->A - r2;

	this->A = result;

	this->F = 64;
	this->F += result == 0 ? 128 : 0;
	this->F += (carry_per_bit >> 3) & 1 ? 32 : 0;
	this->F += (carry_per_bit >> 7) & 1 ? 16 : 0;

}

void CPU::SUBHL(uint8_t& r1, uint8_t& r2)
{
	uint8_t data = read((this->H << 4) + this->L);
	uint8_t result = this->A - data, carry_per_bit = this->A - data;

	this->A = result;

	this->F = 64;
	this->F += result == 0 ? 128 : 0;
	this->F += (carry_per_bit >> 3) & 1 ? 32 : 0;
	this->F += (carry_per_bit >> 7) & 1 ? 16 : 0;
}

void CPU::SUBI(uint8_t& r1, uint8_t& r2)
{
	uint8_t  n = read(PC++);
	uint8_t result = this->A - n, carry_per_bit = this->A - n;

	A = result;

	this->F = 64;
	this->F += result == 0 ? 128 : 0;
	this->F += (carry_per_bit >> 3) & 1 ? 32 : 0;
	this->F += (carry_per_bit >> 7) & 1 ? 16 : 0;
}

void CPU::SUBC(uint8_t& r1, uint8_t& r2)
{
	uint8_t result = this->A - r2 - (this->F & 0b00010000), carry_per_bit = this->A - r2 - (this->F & 0b00010000);

	this->A = result;

	this->F = 64;
	this->F += result == 0 ? 128 : 0;
	this->F += (carry_per_bit >> 3) & 1 ? 32 : 0;
	this->F += (carry_per_bit >> 7) & 1 ? 16 : 0;
}

void CPU::SUBCHL(uint8_t& r1, uint8_t& r2)
{
	uint8_t data = read((this->H << 4) + this->L);

	uint8_t result = this->A - data - (this->F & 0b00010000), carry_per_bit = this->A - data - (this->F & 0b00010000);

	this->A = result;

	this->F = 64;
	this->F += result == 0 ? 128 : 0;
	this->F += (carry_per_bit >> 3) & 1 ? 32 : 0;
	this->F += (carry_per_bit >> 7) & 1 ? 16 : 0;
}

void CPU::SUBCI(uint8_t& r1, uint8_t& r2)
{
	uint8_t n = read(PC++);
	uint8_t result = A - n - (this->F & 0b00010000), carry_per_bit = A - n - (this->F & 0b00010000);

	this->A = result;

	this->F = 64;
	this->F += result == 0 ? 128 : 0;
	this->F += (carry_per_bit >> 3) & 1 ? 32 : 0;
	this->F += (carry_per_bit >> 7) & 1 ? 16 : 0;
}

void CPU::CP(uint8_t& r1, uint8_t& r2)
{
	uint8_t result = this->A - r2, carry_per_bit = this->A - r2;

	this->F = 64;
	this->F += result == 0 ? 128 : 0;
	this->F += (carry_per_bit >> 3) & 1 ? 32 : 0;
	this->F += (carry_per_bit >> 7) & 1 ? 16 : 0;
}

void CPU::CPHL(uint8_t& r1, uint8_t& r2)
{
	uint8_t data = read((this->H << 4) + this->L);
	uint8_t result = A - data, carry_per_bit = A - data;

	this->F = 64;
	this->F += result == 0 ? 128 : 0;
	this->F += (carry_per_bit >> 3) & 1 ? 32 : 0;
	this->F += (carry_per_bit >> 7) & 1 ? 16 : 0;
}

void CPU::CPI(uint8_t& r1, uint8_t& r2)
{
	uint8_t n = read(PC++);
	uint8_t result = A - n, carry_per_bit = A - n;

	this->F = 64;
	this->F += result == 0 ? 128 : 0;
	this->F += (carry_per_bit >> 3) & 1 ? 32 : 0;
	this->F += (carry_per_bit >> 7) & 1 ? 16 : 0;
}

void CPU::INC(uint8_t& r1, uint8_t& r2)
{
	uint8_t result = r1 + 1, carry_per_bit = r1 + 1;

	r1 = result;

	this->F = this->F & 16;

	this->F += result == 0 ? 128 : 0;
	this->F += (carry_per_bit >> 3) & 1 ? 32 : 0;
}

void CPU::INCHL(uint8_t& r1, uint8_t& r2)
{
	uint8_t data = read((this->H << 4) + this->L);

	uint8_t result = data + 1, carry_per_bit = data + 1;

	write((this->H << 4) + this->L, result);

	this->F = this->F & 16;

	this->F += result == 0 ? 128 : 0;
	this->F += (carry_per_bit >> 3) & 1 ? 32 : 0;
}

void CPU::DEC(uint8_t& r1, uint8_t& r2)
{
	uint8_t result = r1 - 1, carry_per_bit = r1 - 1;

	r1 = result;

	this->F = this->F & 16;

	this->F += result == 0 ? 128 : 0;
	this->F += 64;
	this->F += (carry_per_bit >> 3) & 1 ? 32 : 0;
}

void CPU::DECHL(uint8_t& r1, uint8_t& r2)
{
	uint8_t data = read((this->H << 4) + this->L);
	uint8_t result = data - 1, carry_per_bit = data - 1;
	write((this->H << 4) + this->L, result);

	this->F = this->F & 16;

	this->F += result == 0 ? 128 : 0;
	this->F += 64;
	this->F += (carry_per_bit >> 3) & 1 ? 32 : 0;
}

void CPU::LDB(uint8_t& r1,uint8_t& r2)
{
	this->B = read(PC++);
};

void CPU::LDC(uint8_t& r1, uint8_t& r2)
{
	this->C = read(PC++);
};

void CPU::LDD(uint8_t& r1, uint8_t& r2)
{
	this->D = read(PC++);
};

void CPU::LDE(uint8_t& r1, uint8_t& r2)
{
	this->E = read(PC++);
};

void CPU::LDH(uint8_t& r1, uint8_t& r2)
{
	this->H = read(PC++);
};

void CPU::LDL(uint8_t& r1 , uint8_t& r2)
{
	this->L = read(PC++);
};

void CPU::LOAD(uint8_t& r1, uint8_t& r2)
{
	r1 = r2;
};

void CPU::LOAD16B(uint8_t& r1, uint8_t& r2)
{
	if (&r2 == &this->H)
		r1 = read((this->H << 4) + this->L);
	if (&r2 == &this->B)
		r1 = read((this->B << 4) + this->C);
	if (&r2 == &this->D)
		r1 = read((this->D << 4) + this->E);
}

void CPU::LOADHLMIN(uint8_t& r1, uint8_t& r2)
{
	LOADIN16B(r1, r2);
	uint16_t value = (this->H << 8) + this->L;
	value--;
	this->L = value & 0x00FF;
	this->H = value & 0xFF00;
}

void CPU::LOADHLADD(uint8_t& r1, uint8_t& r2)
{
	LOADIN16B(r1, r2);
	uint16_t value = (this->H << 8) + this->L;
	value++;
	this->L = value & 0x00FF;
	this->H = value & 0xFF00;
}

void CPU::LOADAHLMIN(uint8_t& r1, uint8_t& r2)
{
	LOAD16B(r1, r2);
	uint16_t value = (this->H << 8) + this->L;
	value--;
	this->L = value & 0x00FF;
	this->H = value & 0xFF00;
}

void CPU::LOADAHLADD(uint8_t& r1, uint8_t& r2)
{
	LOAD16B(r1, r2);
	uint16_t value = (this->H << 8) + this->L;
	value++;
	this->L = value & 0x00FF;
	this->H = value & 0xFF00;
}

void CPU::LOADSP16B(uint8_t& r1, uint8_t& r2)
{
	if (&r2 == &this->H)
		this->SP = (this->H << 4) + this->L;
	if (&r2 == &this->B)
		this->SP = (this->B << 4) + this->C;
	if (&r2 == &this->D)
		this->SP = (this->D << 4) + this->E;
}

void CPU::LOADIN16B(uint8_t& r1, uint8_t& r2)
{
	if (&r1 == &this->H)
		write((this->H << 4) + this->L, r2);
	if (&r1 == &this->B)
		write((this->B << 4) + this->C, r2);
	if (&r1 == &this->D)
		write((this->D << 4) + this->E, r2);
}

void CPU::LOADACFF00(uint8_t& r1, uint8_t& r2)
{
	r1 = read(0xFF00 + this->C);
}

void CPU::LOADCFF00A(uint8_t& r1, uint8_t& r2)
{
	write(0xFF00 + this->C, r2);
}

void CPU::LOADnA(uint8_t& r1, uint8_t& r2)
{
	uint8_t n = read(PC++);
	write(0xFF00 + n, r2);
}

void CPU::LOADAn(uint8_t& r1, uint8_t& r2)
{
	uint8_t n = read(PC++);
	this->A = read(0xFF00 + n);
}

void CPU::LOADIN16BRnn(uint8_t& r1, uint8_t& r2)
{
	uint8_t nn_lsb = read(PC++);
	uint8_t nn_msb = read(PC++);

	if (&r1 == &this->H)
	{
		this->H = nn_msb;
		this->L = nn_lsb;
	}
	if (&r1 == &this->B)
	{
		this->B = nn_msb;
		this->C = nn_lsb;
	}
	if (&r1 == &this->D)
	{
		this->D = nn_msb;
		this->E = nn_lsb;
	}
	if (r1 == NULL)
	{
		this->SP = (nn_msb << 8) + nn_lsb;
	}
}

void CPU::LOADHLSPE(uint8_t& r1, uint8_t& r2)
{
	uint8_t e = read(PC++);
	uint16_t result = this->SP + e, carry_per_bit = this->SP + e;

	this->H = result & 0xFF00;
	this->L = result & 0xFF;

	this->F = 0;

	// if (carry_per_bit[3] then set H(ie add 32))
	this->F += (carry_per_bit >> 3) & 1 ? 32 : 0;
	this->F += (carry_per_bit >> 7) & 1 ? 16 : 0;
}

void CPU::LOADnnA(uint8_t& r1, uint8_t& r2)
{
	uint8_t nn_lsb = read(PC++);
	uint8_t nn_msb = read(PC++);
	write((nn_msb << 8) + nn_lsb, r2);
}

void CPU::LOADnnSP(uint8_t& r1, uint8_t& r2)
{
	uint8_t nn_lsb = read(PC++);
	uint8_t nn_msb = read(PC++);

	uint8_t v1 = this->SP & 0x00FF;
	uint8_t v2 = (this->SP & 0xFF00) >> 4;

	write((nn_msb << 4) + nn_lsb, v1);
	write((nn_msb << 4) + nn_lsb + 1, v2);
}

void CPU::PushSPrr(uint8_t& r1, uint8_t& r2)
{
	this->SP--;
	write(static_cast<uint16_t&&>(this->SP),r1);
	this->SP--;
	write(static_cast<uint16_t&&>(this->SP), r2);

}

void CPU::PoprrSP(uint8_t& r1, uint8_t& r2)
{
	uint8_t nn_lsb = read(SP++);
	uint8_t nn_msb = read(SP++);
	r1 = nn_msb;
	r2 = nn_lsb;
}