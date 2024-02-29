#pragma once
#include <iostream>
#include <sstream>
#include <fstream>

#define IS_NUM(x, is_reg) ((is_reg) ? std::string("") : std::string("0x")) + numToHex(static_cast<int>(x))
#define IS_PTR(x) std::string("[0x") + numToHex(static_cast<int>(x)) + "]"

using uByte = unsigned char;
using uWord = unsigned short;

namespace Disassembler
{
	inline std::string numToHex(size_t num);
	uWord instruction_to_num(uByte i1, uByte i2, uByte i3, uByte i4);

	typedef struct
	{

		uByte FirstQuarter : 4;
		uByte SecondQuarter : 4;
		uByte ThirdQuarter : 4;
		uByte FourthQuarter : 4;

	} Instruction;

	inline void loadNumberToBuffer(uWord& buffer, uByte N1);
	inline void loadNumberToBuffer(uWord& buffer, uByte N1, uByte N2);
	inline void loadNumberToBuffer(uWord& buffer, uByte N1, uByte N2, uByte N3);
	inline void clearBuffer(uWord& buffer);

	void loadInstruction(Instruction& instruction, char* buffer, size_t index);

	void disassembly(char* buffer, size_t size);

	std::string extractInstructions(Instruction& instruction, uWord& buffer);
}; //namespace: Disassembler

