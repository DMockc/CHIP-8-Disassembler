#include "Disassembler.h"

namespace Disassembler
{
	inline std::string numToHex(size_t num)
	{
		std::ostringstream stream;
		stream << std::hex << std::uppercase << num;
		return stream.str();
	}

	inline void loadNumberToBuffer(uWord& buffer, uByte N1)
	{
		buffer |= N1;
	}

	inline void loadNumberToBuffer(uWord& buffer, uByte N1, uByte N2)
	{
		buffer = (N1 << 4) | N2;
	}

	inline void loadNumberToBuffer(uWord& buffer, uByte N1, uByte N2, uByte N3)
	{
		buffer = (N1 << 8) | ((N2 << 4) | N3);
	}

	inline void clearBuffer(uWord& buffer)
	{
		buffer = 0;
	}

	uWord instruction_to_num(uByte i1, uByte i2, uByte i3, uByte i4)
	{
		uWord fullInstruction = 0;

		fullInstruction |= (i1 << 12);
		fullInstruction |= (i2 << 8);
		fullInstruction |= (i3 << 4);
		fullInstruction |= i4;
		return fullInstruction;
	}

	void loadInstruction(Instruction& instruction, char* buffer, size_t index)
	{
		uByte inst = static_cast<uByte>(buffer[index]);

		instruction.FirstQuarter = 0;
		instruction.SecondQuarter = 0;
		instruction.ThirdQuarter = 0;
		instruction.FourthQuarter = 0;


		instruction.FirstQuarter |= (inst >> 4);
		instruction.SecondQuarter = 0x0F;
		instruction.SecondQuarter &= inst;


		inst = static_cast<uByte>(buffer[index + 1]);
		instruction.ThirdQuarter |= (inst >> 4);
		instruction.FourthQuarter = 0x0F;
		instruction.FourthQuarter &= inst;
	}

	void disassembly(char* buffer, size_t size)
	{
		std::ofstream outputFile("Disassembly.asmc8");

		if (!outputFile.is_open())
		{
			std::cerr << "An error has ocurred!\n";
			exit(-1);
		}

		Instruction currentInstruction;
		uWord operation_buffer;
		std::string inst_code;
		for (size_t i = 0; i + 1 < size; i += 2)
		{
			Disassembler::loadInstruction(currentInstruction, buffer, i);
			inst_code = Disassembler::extractInstructions(currentInstruction, operation_buffer);
			std::cout << inst_code << "  |  " << i + 1 << "\n";
			outputFile << inst_code + "    ; 0x" + numToHex(0x200 + i) + "\n";
			clearBuffer(operation_buffer);
		}
	}

	std::string extractInstructions(Instruction& instruction, uWord& buffer)
	{
		switch (instruction.FirstQuarter)
		{
		case 0x0:

			switch (instruction.FourthQuarter)
			{

			case 0x0: //CLS (00E0): Clear the display by setting all pixels to black.

				return "CLS";

				break;


			case 0xE: //RET (00EE): Return from a subroutine. Pops the value at the top of the stack (indicated by the stack pointer SP) and puts it in PC

				return "RET";

				break;


			default: //Unknown instruction
				return "[ERROR] Unknown instruction: " + IS_NUM(instruction_to_num(static_cast<int>(instruction.FirstQuarter),
					static_cast<int>(instruction.SecondQuarter),
					static_cast<int>(instruction.ThirdQuarter),
					static_cast<int>(instruction.FourthQuarter)), false);
				break;
			}

			break;


		case 0x1: //JMP (1NNN): Jump to address in NNN. Sets the PC to NNN
			loadNumberToBuffer(buffer, instruction.SecondQuarter, instruction.ThirdQuarter, instruction.FourthQuarter);
			return "JMP " + IS_PTR(buffer);
			break;


		case 0x2: //CALL NNN (2NNN): Call the subroutine at address NNN. It increments SP, puts the current PC at the top of the stack and sets PC to the address NNN
			loadNumberToBuffer(buffer, instruction.SecondQuarter, instruction.ThirdQuarter, instruction.FourthQuarter);
			return "CALL " + IS_PTR(buffer);
			break;


		case 0x3: //SE VX, NN (3XNN): Skip the next instruction if VX == NN. Compare the value of register VX with NN and if they are equal, increment PC by two.
			loadNumberToBuffer(buffer, instruction.ThirdQuarter, instruction.FourthQuarter);
			return "SE V" + IS_NUM(instruction.SecondQuarter, true) +
				", " + IS_NUM(buffer, false);
			break;


		case 0x4: //SNE VX, NN (4XNN): Skip the next instruction if VX != NN. Compare the value of register VX with NN and if they are not equal, increment PC by two.
			loadNumberToBuffer(buffer, instruction.ThirdQuarter, instruction.FourthQuarter);
			return "SNE V" + IS_NUM(instruction.SecondQuarter, true) +
				", " + IS_NUM(buffer, false);
			break;


		case 0x5: //SE VX, VY (5XY0): Skip the next instruction if VX == VY. Compare the value of register VX with the value of VY and if they are equal, increment PC by two.
			return "SE V" + IS_NUM(instruction.SecondQuarter, true) +
				", V" + IS_NUM(instruction.ThirdQuarter, true);
			break;


		case 0x6: //LD VX, NN (6XNN): Load the value NN into the register VX
			loadNumberToBuffer(buffer, instruction.ThirdQuarter, instruction.FourthQuarter);
			return "LD V" + IS_NUM(instruction.SecondQuarter, true) +
				", " + IS_NUM(buffer, false);
			break;


		case 0x7: //ADD VX, NN (7XNN): Add the value NN to the value of register VX and store the result in VX
			loadNumberToBuffer(buffer, instruction.ThirdQuarter, instruction.FourthQuarter);
			return "ADD V" + IS_NUM(instruction.SecondQuarter, true) +
				", " + IS_NUM(buffer, false);
			break;

		case 0x8: //Bitwise operations
			switch (instruction.FourthQuarter)
			{
			case 0x0: //LD VX, VY (8XY0): Put the value of register VY into VX
				return "LD V" + IS_NUM(instruction.SecondQuarter, true) +
					", V" + IS_NUM(instruction.ThirdQuarter, true);
				break;


			case 0x1: //OR VX, VY (8XV1): Perform a bitwise OR between the values of VX and VY and store the result in VX
				return "OR V" + IS_NUM(instruction.SecondQuarter, true) +
					", V" + IS_NUM(instruction.ThirdQuarter, true);
				break;


			case 0x2: //AND VX, VY (8XY2): Perform a bitwise AND between the values of VX and VY and store the result in VX
				return "AND V" + IS_NUM(instruction.SecondQuarter, true) +
					", V" + IS_NUM(instruction.ThirdQuarter, true);
				break;


			case 0x3: //XOR VX, VY (8XY3): Perform a bitwise XOR between the values of VX and VY and store the result in VX
				return "XOR V" + IS_NUM(instruction.SecondQuarter, true) +
					", V" + IS_NUM(instruction.ThirdQuarter, true);
				break;


			case 0x4: //ADD VX, VY (8XY4): Add the values of VX and VY and store the result in VX. Put the carry bit in VF (if there is overflow, set VF to 1, otherwise 0)
				return "ADD V" + IS_NUM(instruction.SecondQuarter, true) +
					", V" + IS_NUM(instruction.ThirdQuarter, true);
				break;


			case 0x5: //SUB VX, VY (8XY5): Subtract the value of VY from VX and store the result in VX. Put the borrow in VF (if there is borrow, VX > VY, set VF to 1, otherwise 0)
				return "SUB V" + IS_NUM(instruction.SecondQuarter, true) +
					", V" + IS_NUM(instruction.ThirdQuarter, true);
				break;


			case 0x6: //SHR VX {, VY} (8XY6): Shift right, or divide VX by two. Store the least significant bit of VX in VF, and then divide VX and store its value in VX
				return "SHR V" + IS_NUM(instruction.SecondQuarter, true) +
					", V" + IS_NUM(instruction.ThirdQuarter, true);
				break;


			case 0x7: //SUBN VX, VY (8XY7): Subtract the value of VY from VX and store the result in VX. Set VF to 1 if there is no borrow, to 0 otherwise
				return "SUBN V" + IS_NUM(instruction.SecondQuarter, true) +
					", V" + IS_NUM(instruction.ThirdQuarter, true);
				break;


			case 0xE: //SHL VX {, VY} (8XYE): Shift left, or multiply VX by two. Store the most significant bit of VX in VF, and then multiply VX and store its value in VX
				return "SHL V" + IS_NUM(instruction.SecondQuarter, true) +
					", V" + IS_NUM(instruction.ThirdQuarter, true);
				break;


			default:
				return "[ERROR] Unknown instruction: " + IS_NUM(instruction_to_num(static_cast<int>(instruction.FirstQuarter),
					static_cast<int>(instruction.SecondQuarter),
					static_cast<int>(instruction.ThirdQuarter),
					static_cast<int>(instruction.FourthQuarter)), false);
				break;
			}

			break;

		case 0x9: //SNE VX, VY (9XY0): Skip the next instruction if the values of VX and VY are not equal
			return "SNE V" + IS_NUM(instruction.SecondQuarter, true) +
				", V" + IS_NUM(instruction.ThirdQuarter, true);
			break;


		case 0xA: //LD I, NNN (ANNN): Set the value of I to the address NNN
			loadNumberToBuffer(buffer, instruction.SecondQuarter, instruction.ThirdQuarter, instruction.FourthQuarter);
			return "LD I, " + IS_NUM(buffer, false);
			break;


		case 0xB: //JMP V0, NNN (BNNN): Jump to the location NNN + V0
			loadNumberToBuffer(buffer, instruction.SecondQuarter, instruction.ThirdQuarter, instruction.FourthQuarter);
			return "LD V0, " + IS_NUM(buffer, false);
			break;


		case 0xC: //RND VX, NN (CXNN): Generate a random byte (from 0 to 255), do a bitwise AND with NN and store the result to VX
			loadNumberToBuffer(buffer, instruction.ThirdQuarter, instruction.FourthQuarter);
			return "RND V" + IS_NUM(instruction.SecondQuarter, true) +
				", " + IS_NUM(buffer, false);
			break;


		case 0xD: /*DRW VX, VY, N (DXYN):
			* The draw instruction. This is arguably the most involved operation
			* The n-byte sprite starting at the address I is drawn to the display at the coordinates [VX, VY]
			* Then, set VF to 1 if there has been a collision (a display bit was changed from 1 to 0)
			* The interpreter must read N bytes from the I address in memory
			* These bytes are interpreted as a sprite and drawn at the display coordinates [VX, VY]
			* The bits are set using an XOR with the current display state
			*/

			return "DRW V" + IS_NUM(instruction.SecondQuarter, true) +
				", V" + IS_NUM(instruction.ThirdQuarter, true) +
				", " + IS_NUM(instruction.FourthQuarter, false);

			break;


		case 0xE:
			switch (instruction.FourthQuarter)
			{
			case 0xE: /*SKP VX (EX9E) : Skip the next instruction if the key with the value of VX is currently pressed.
			* Basically, increase PC by two if the key corresponding to the value in VX is pressed. */
				return "SKP V" + IS_NUM(instruction.SecondQuarter, true);

				break;

			case 0x1: /* SKNP VX(EXA1) : Skip the next instruction if the key with the value of VX is currently not pressed.
				* Basically, increase PC by two if the key corresponding to the value in VX is not pressed. */
				return "SKNP V" + IS_NUM(instruction.SecondQuarter, true);

				break;


			default:
				return "[ERROR] Unknown instruction: " + IS_NUM(instruction_to_num(static_cast<int>(instruction.FirstQuarter),
					static_cast<int>(instruction.SecondQuarter),
					static_cast<int>(instruction.ThirdQuarter),
					static_cast<int>(instruction.FourthQuarter)), false);
				break;
			}

			break;


		case 0xF:
			loadNumberToBuffer(buffer, instruction.ThirdQuarter, instruction.FourthQuarter);
			switch (buffer)
			{
			case 0x07: //LD VX, DT (FX07): Read the delay timer register value into VX
				return "LD V" + IS_NUM(instruction.SecondQuarter, true) +
					", DT";
				break;


			case 0x0A: //LD VX, K (FX0A): Wait for a key press, and then store the value of the key to VX
				return "LD V" + IS_NUM(instruction.SecondQuarter, true) +
					", K";

				break;


			case 0x15: //LD DT, VX (FX15): Load the value of VX into the delay timer DT
				return "LD DT, V" + IS_NUM(instruction.SecondQuarter, true);

				break;


			case 0x18: //LD ST, VX (FX18): Load the value of VX into the sound time ST
				return "LD ST, V" + IS_NUM(instruction.SecondQuarter, true);

				break;


			case 0x1E: //ADD I, VX (FX1E): Add the values of I and VX, and store the result in I
				return "ADD I, V" + IS_NUM(instruction.SecondQuarter, true);

				break;


			case 0x29: /* LD F, VX (FX29): Set the location of the sprite for the digit VX to I.
				The font sprites start at address 0x000, and contain the hexadecimal digits from 1..F. Each font has a length of 0x05 bytes.
				The memory address for the value in VX is put in I. */
				return "LD F, V" + IS_NUM(instruction.SecondQuarter, true);

				break;


			case 0x33: /* LD B, VX (FX33): Store the binary-coded decimal in VX and put it in three consecutive memory slots starting at I.
				* VX is a byte, so it is in 0…255. The interpreter takes the value in VX (for example the decimal value 174, or 0xAE in hex),
				* converts it into a decimal and separates the hundreds, the tens and the ones (1, 7 and 4 respectively).
				* Then, it stores them in three memory locations starting at I (1 to I, 7 to I+1 and 4 to I+2)*/
				return "LD B, V" + IS_NUM(instruction.SecondQuarter, true);

				break;


			case 0x55: /* LD[I], VX (FX55) : Store registers from V0 to VX in the main memory, starting at location I.
				Note that X is the number of the register, so we can use it in the loop.
				*/
				return "LD[I], V" + IS_NUM(instruction.SecondQuarter, true);

				break;


			case 0x65: // LD VX, [I] (FX65): Load the memory data starting at address I into the registers V0 to VX
				return "LD V" + IS_NUM(instruction.SecondQuarter, true) +
					", [I]";

				break;

			default:
				return "[ERROR] Unknown instruction: " + IS_NUM(instruction_to_num(static_cast<int>(instruction.FirstQuarter),
					static_cast<int>(instruction.SecondQuarter),
					static_cast<int>(instruction.ThirdQuarter),
					static_cast<int>(instruction.FourthQuarter)), false);
				break;
			}

			break;


		default:
			return "[ERROR] Unknown instruction: " + IS_NUM(instruction_to_num(static_cast<int>(instruction.FirstQuarter),
				static_cast<int>(instruction.SecondQuarter),
				static_cast<int>(instruction.ThirdQuarter),
				static_cast<int>(instruction.FourthQuarter)), false);
			break;
		}

		return "";
	}

}; //namespace: Disassembler