#include <iostream>
#include "CPU.h"

//https://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html for opcode table

int main()
{
	CPU cpu;
	while (true)
		cpu.decode();

	return 0;

}