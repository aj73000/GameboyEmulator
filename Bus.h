#include <vector>
#include <iostream>
/*Bus
* DESCRIPTION: This is the definition of the Bus class that is used atm for storing the memory
*/
class Bus
{
	//  BIT : 8:8,8:8
	// {intruction:data,intruction:data}
public:
	//(size, value)
	//std::vector<uint8_t> ram = std::vector<uint8_t>(0xFFFF,0x00);
	std::vector<uint8_t> ram = { 0x06,'B',0x06,'A',0x06,'C' };


public:
	uint8_t read(uint16_t& address)
	{
		return ram[address];
	}

	void write(uint16_t& address, uint8_t data)
	{
		ram[address] = data;
	}
};