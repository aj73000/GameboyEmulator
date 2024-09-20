#pragma once
#include <cstdint>
#include <vector>
#include "Bus.h"
#include <string>
#include <cassert>
#include <iostream>

class CPU
{
	//Instructions are in the form 4bit for op 4bit for the data
	// LDA 01
	// OP Data
	Bus* bus = new Bus();

	//The instruction bytes
	uint8_t opcode = 0x00;
	uint8_t data = 0x00;

	int cycles = 0;

	//Registers
	//Corrisponding values can be combined AF,BC,DE,HL

	//15...8
	uint8_t A = 0;
	uint8_t B = 'A';
	uint8_t D = 0;
	uint8_t H = 0;

	//7...0
	uint8_t F = 0b11110000;

	//Flags (Low 8 bits of the AF)
	/*
	*  z - Zero Flag
	*  n - Sub flag
	*  h - Half carry flag
	*  c - Carry flag
	*/

	uint8_t C = 0;
	uint8_t E = 0;
	uint8_t L = 0;

	//Current stack position
	uint16_t SP = 0;
	//Next instruction to be executed //0x100;
	// 
	//uint16_t PC = 0x0100;
	uint16_t PC = 0x0000;

	struct INSTRUCTION
	{
		std::string name;

		uint8_t* value1;
		uint8_t* value2;
		//Data to be processed
		//uint16_t(CPU::*opperand)(void) = nullptr;

		//What is to be performed
		void (CPU::* opcode)(uint8_t&, uint8_t&) = nullptr;

		int numCycles;
	};

	//After instruction is decoded this table must be searched
	std::unique_ptr<std::vector<INSTRUCTION>> lookuptable = std::unique_ptr<std::vector<INSTRUCTION>>(new std::vector<INSTRUCTION>
		{
			{}, { "LDBCd16",&this->B,NULL,&CPU::LOADIN16BRnn,12 }, { "LDBCA",&this->B,&this->A,&CPU::LOADIN16B,8 }, {}, {}, {}, { "LDB",NULL,NULL,&CPU::LDB,8 }, {}, { "LDnnSP",NULL,NULL,&CPU::LOADnnSP,20 }, {}, { "LDABC" }, {}, {}, {}, { "LDC",NULL,NULL,&CPU::LDC,8 }, {},
			{}, { "LDDEd16",&this->D,NULL,&CPU::LOADIN16BRnn,12 }, { "LDDEA",&this->D,&this->A,&CPU::LOADIN16B,8 }, {}, {}, {}, { "LDD",NULL,NULL,&CPU::LDD,8 }, {}, {}, {}, { "LDADE" }, {}, {}, {}, { "LDE",NULL,NULL,&CPU::LDE,8 }, {},
			{}, { "LDHLd16",&this->H,NULL,&CPU::LOADIN16BRnn,12 }, { "LDHL+",&this->H,&this->A,&CPU::LOADHLADD,8 }, {}, {}, {}, { "LDH",NULL,NULL,&CPU::LDH,8 }, {}, {}, {}, { "LDAHL+",&this->A,&this->H,&CPU::LOADAHLADD,8 }, {}, {}, {}, { "LDL",NULL,NULL,&CPU::LDL,8 }, {},
			{}, { "LDSPd16",NULL,NULL,&CPU::LOADIN16BRnn,12 }, { "LDHL-",&this->H,&this->A,&CPU::LOADHLMIN,8 }, {}, {}, {}, {}, {}, {}, {}, { "LDAHL-",&this->A,&this->H,&CPU::LOADAHLMIN,8 }, {}, {}, {}, {}, {},
			{ "LDBB",&this->B,&this->B,&CPU::LOAD,4 }, { "LDBC",&this->B,&this->C,&CPU::LOAD,4 }, { "LDBD",&this->B,&this->D,&CPU::LOAD,4 }, { "LDBE",&this->B,&this->E,&CPU::LOAD,4 }, { "LDBH",&this->B,&this->H,&CPU::LOAD,4 }, { "LDBL",&this->B,&this->L,&CPU::LOAD,4 }, { "LDBHL",&this->B,&this->H,&CPU::LOAD16B,8 }, { "LDBA",&this->B,&this->A,&CPU::LOAD,4 }, { "LDCB",&this->C,&this->B,&CPU::LOAD,4 }, { "LDCC",&this->C,&this->C,&CPU::LOAD,4 }, { "LDCD",&this->C,&this->D,&CPU::LOAD,4 }, { "LDCE",&this->C,&this->E,&CPU::LOAD,4 }, { "LDCH",&this->C,&this->H,&CPU::LOAD,4 }, { "LDCL",&this->C,&this->L,&CPU::LOAD,4 }, { "LDCHL",&this->C,&this->H,&CPU::LOAD16B,8 }, { "LDCA",&this->C,&this->A,&CPU::LOAD,4 },
			{ "LDDB",&this->D,&this->B,&CPU::LOAD,4 }, { "LDDC",&this->D,&this->C,&CPU::LOAD,4 }, { "LDDD",&this->D,&this->D,&CPU::LOAD,4 }, { "LDDE",&this->D,&this->E,&CPU::LOAD,4 }, { "LDDH",&this->D,&this->H,&CPU::LOAD,4 }, { "LDDL",&this->D,&this->L,&CPU::LOAD,4 }, { "LDDHL",&this->D,&this->H,&CPU::LOAD16B,8 }, { "LDDA",&this->D,&this->A,&CPU::LOAD,4 }, { "LDEB",&this->E,&this->B,&CPU::LOAD,4 }, { "LDEC",&this->E,&this->C,&CPU::LOAD,4 }, { "LDED",&this->E,&this->D,&CPU::LOAD,4 }, { "LDEE",&this->E,&this->E,&CPU::LOAD,4 }, { "LDEH",&this->E,&this->H,&CPU::LOAD,4 }, { "LDEL",&this->E,&this->L,&CPU::LOAD,4 }, { "LDEHL",&this->E,&this->H,&CPU::LOAD16B,8 }, { "LDEA",&this->E,&this->A,&CPU::LOAD,4 },
			{ "LDHB",&this->H,&this->B,&CPU::LOAD,4 }, { "LDHC",&this->H,&this->C,&CPU::LOAD,4 }, { "LDHD",&this->H,&this->D,&CPU::LOAD,4 }, { "LDHE",&this->H,&this->E,&CPU::LOAD,4 }, { "LDHH",&this->H,&this->H,&CPU::LOAD,4 }, { "LDHL",&this->H,&this->L,&CPU::LOAD,4 }, { "LDHHL",&this->H,&this->H,&CPU::LOAD16B,8 }, { "LDHA",&this->H,&this->A,&CPU::LOAD,4 }, { "LDLB",&this->L,&this->B,&CPU::LOAD,4 }, { "LDLC",&this->L,&this->C,&CPU::LOAD,4 }, { "LDLD",&this->L,&this->D,&CPU::LOAD,4 }, { "LDLE",&this->L,&this->E,&CPU::LOAD,4 }, { "LDLH",&this->L,&this->H,&CPU::LOAD,4 }, { "LDLL",&this->L,&this->L,&CPU::LOAD,4 }, { "LDLHL",&this->L,&this->H,&CPU::LOAD16B,8 }, { "LDLA",&this->L,&this->A,&CPU::LOAD,4 },
			{}, {}, {}, {}, {}, {}, {}, { "LDHLA",&this->H,&this->A,&CPU::LOADIN16B,8 }, { "LDAB",&this->A,&this->B,&CPU::LOAD,4 }, { "LDAC",&this->A,&this->C,&CPU::LOAD,4 }, { "LDAD",&this->A,&this->D,&CPU::LOAD,4 }, { "LDAE",&this->A,&this->E,&CPU::LOAD,4 }, { "LDAH",&this->A,&this->H,&CPU::LOAD,4 }, { "LDAL",&this->A,&this->L,&CPU::LOAD,4 }, { "LDAHL",&this->A,&this->H,&CPU::LOAD16B,8 }, { "LDAA",&this->A,&this->A,&CPU::LOAD,4 },
			{"ADDAB",&this->A,&this->B,&CPU::ADD,4}, {"ADDAC",&this->A,&this->C,&CPU::ADD,4}, {"ADDAD",&this->A,&this->D,&CPU::ADD,4}, {"ADDAE",&this->A,&this->E,&CPU::ADD,4}, {"ADDAH",&this->A,&this->H,&CPU::ADD,4}, {"ADDAL",&this->A,&this->L,&CPU::ADD,4}, {"ADDAHL",&this->A,NULL,&CPU::ADDHL,8}, {"ADDAA",&this->A,&this->A,&CPU::ADD,4}, {"ADCAB",NULL,&this->B,&CPU::ADC,4}, {"ADCAC",NULL,&this->C,&CPU::ADC,4}, {"ADCAD",NULL,&this->D,&CPU::ADC,4}, {"ADCAE",NULL,&this->E,&CPU::ADC,4}, {"ADCAH",NULL,&this->H,&CPU::ADC,4}, {"ADCAL",NULL,&this->L,&CPU::ADC,4}, {"ADCAHL",NULL,NULL,&CPU::ADCHL,8}, {"ADCAA",NULL,&this->A,&CPU::ADC,4},
			{"SUBB",NULL,&this->B,&CPU::SUB,4}, {"SUBC",NULL,&this->C,&CPU::SUB,4}, {"SUBD",NULL,&this->D,&CPU::SUB,4}, {"SUBE",NULL,&this->E,&CPU::SUB,4}, {"SUBH",NULL,&this->H,&CPU::SUB,4}, {"SUBL",NULL,&this->L,&CPU::SUB,4}, {"SUBHL",NULL,NULL,&CPU::SUBHL,8}, {"SUBA",NULL,&this->A,&CPU::SUB,4}, {}, {}, {}, {}, {}, {}, {}, {},
			{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
			{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
			{}, { "PopBCSP",&this->B,&this->C,&CPU::PoprrSP,12 }, {}, {}, {}, { "PushSPBC",&this->B,&this->C,&CPU::PushSPrr,16 }, {"ADDAI",&this->A,NULL,&CPU::ADDI,8}, {}, {}, {}, {}, {}, {}, {}, {"ADCI",NULL,NULL,&CPU::ADCI,8}, {},
			{}, { "PopDESP",&this->D,&this->E,&CPU::PoprrSP,12 }, {}, {}, {}, { "PushSPDE",&this->D,&this->E,&CPU::PushSPrr,16 }, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
			{ "LDHFF00nA",NULL,&this->A,&CPU::LOADnA,12 }, { "PopHLSP",&this->H,&this->L,&CPU::PoprrSP,12 }, { "LDCFF00A",NULL,&this->A,&CPU::LOADCFF00A,8 }, {}, {}, { "PushSPHL",&this->H,&this->L,&CPU::PushSPrr,16 }, {}, {}, {}, {}, { "LDnnA",NULL,&this->A,&CPU::LOADnnA,16 }, {}, {}, {}, {}, {},
			{ "LDHAn",NULL,NULL,&CPU::LOADAn,12 }, { "PopAFSP",&this->A,&this->F,&CPU::PoprrSP,12 }, { "LDACFF00",&this->A,NULL,&CPU::LOADACFF00,8 }, {}, {}, { "PushSPAF",&this->A,&this->F,&CPU::PushSPrr,16 }, {}, {}, {"LDHLSPE",NULL,NULL,&CPU::LOADHLSPE,12}, {"LDSPHL",NULL,&this->H,&CPU::LOADSP16B,8}, {}, {}, {}, {}, {}, {}
		});

	//Fetch the instruction
public:
	CPU();
	uint8_t read(uint16_t&& address);

	void write(uint16_t&& addr, uint8_t& data);

	void decode();


private:

	//Reset all the values
	void reset();

	//Opcodes
	
	//Arthimetic and logic operations
	void ADD(uint8_t& r1, uint8_t& r2);
	void ADDHL(uint8_t& r1, uint8_t& r2);
	void ADDI(uint8_t& r1, uint8_t& r2);
	void ADC(uint8_t& r1, uint8_t& r2);
	void ADCHL(uint8_t& r1, uint8_t& r2);
	void ADCI(uint8_t& r1, uint8_t& r2);
	void SUB(uint8_t& r1, uint8_t& r2);
	void SUBHL(uint8_t& r1, uint8_t& r2);

	// //LOADS
	//4 cycle load instructions
	void LDB(uint8_t& r1,uint8_t& r2);
	void LDC(uint8_t& r1, uint8_t& r2);
	void LDD(uint8_t& r1, uint8_t& r2);
	void LDE(uint8_t& r1, uint8_t& r2);
	void LDH(uint8_t& r1, uint8_t& r2);
	void LDL(uint8_t& r1, uint8_t& r2);
	void LOAD(uint8_t& r1, uint8_t& r2);
	void LOAD16B(uint8_t& r1,uint8_t& r2);

	//8 cycle load instructiones
	void LOADIN16B(uint8_t& r1, uint8_t& r2);
	void LOADACFF00(uint8_t& r1, uint8_t& r2);
	void LOADCFF00A(uint8_t& r1, uint8_t& r2);

	void LOADHLMIN(uint8_t& r1, uint8_t& r2);
	void LOADHLADD(uint8_t& r1, uint8_t& r2);
	void LOADAHLMIN(uint8_t& r1, uint8_t& r2);
	void LOADAHLADD(uint8_t& r1, uint8_t& r2);

	void LOADSP16B(uint8_t& r1, uint8_t& r2);

	//12 cycle load instructiones
	void LOADnA(uint8_t& r1, uint8_t& r2);
	void LOADAn(uint8_t& r1, uint8_t& r2);
	void LOADIN16BRnn(uint8_t& r1, uint8_t& r2);
	void LOADHLSPE(uint8_t& r1, uint8_t& r2);
	
	//16 cycle load instructions
	void LOADnnA(uint8_t& r1, uint8_t& r2);

	//20 cycle load instructions
	void LOADnnSP(uint8_t& r1, uint8_t& r2);


	//Push and Pop
	void PushSPrr(uint8_t& r1, uint8_t& r2);
	void PoprrSP(uint8_t& r1, uint8_t& r2);
};
