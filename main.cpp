#include <iostream>
#include "CPU.h"

//https://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html for opcode table

int main()
{
	CPU cpu;
	// Will go out of bounds and cause an error atm
	for (int i = 0; i < 8*3; i++)
	{
		cpu.decode();
	}

	/*while (true)
	{
		cpu.decode();
	}*/

	return 0;
}