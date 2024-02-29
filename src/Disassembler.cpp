#include "Disassembler.h"

namespace Disassembler
{
	inline std::string numToHex(size_t num)
	{
		std::ostringstream stream;
		stream << std::hex << std::uppercase << num;
		return stream.str();
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
			outputFile << inst_code + "    ; 0x" << std::hex << (i + 0x200) << "\n";
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

			case CLS:

				return "CLS";

				break;


			case RET:

				return "RET";

				break;


			default:
				return "[ERROR] Unknown instruction: " + IS_NUM(getFullInstruction(static_cast<int>(instruction.FirstQuarter),
					static_cast<int>(instruction.SecondQuarter),
					static_cast<int>(instruction.ThirdQuarter),
					static_cast<int>(instruction.FourthQuarter)), false);
				break;
			}

			break;


		case JMP_NNN:
			loadNumberToBuffer(buffer, instruction.SecondQuarter, instruction.ThirdQuarter, instruction.FourthQuarter);
			return "JMP " + IS_PTR(buffer);
			break;


		case CALL_NNN:
			loadNumberToBuffer(buffer, instruction.SecondQuarter, instruction.ThirdQuarter, instruction.FourthQuarter);
			return "CALL " + IS_PTR(buffer);
			break;


		case SE_VX_NN:
			loadNumberToBuffer(buffer, instruction.ThirdQuarter, instruction.FourthQuarter);
			return "SE V" + IS_NUM(instruction.SecondQuarter, true) +
				", " + IS_NUM(buffer, false);
			break;


		case SNE_VX_NN:
			loadNumberToBuffer(buffer, instruction.ThirdQuarter, instruction.FourthQuarter);
			return "SNE V" + IS_NUM(instruction.SecondQuarter, true) +
				", " + IS_NUM(buffer, false);
			break;


		case SE_VX_VY:
			return "SE V" + IS_NUM(instruction.SecondQuarter, true) +
				", V" + IS_NUM(instruction.ThirdQuarter, true);
			break;


		case LD_VX_NN:
			loadNumberToBuffer(buffer, instruction.ThirdQuarter, instruction.FourthQuarter);
			return "LD V" + IS_NUM(instruction.SecondQuarter, true) +
				", " + IS_NUM(buffer, false);
			break;


		case ADD_VX_NN:
			loadNumberToBuffer(buffer, instruction.ThirdQuarter, instruction.FourthQuarter);
			return "ADD V" + IS_NUM(instruction.SecondQuarter, true) +
				", " + IS_NUM(buffer, false);
			break;

		case BITWISE_OP:
			switch (instruction.FourthQuarter)
			{
			case LD_VX_VY:
				return "LD V" + IS_NUM(instruction.SecondQuarter, true) +
					", V" + IS_NUM(instruction.ThirdQuarter, true);
				break;


			case OR_VX_VY:
				return "OR V" + IS_NUM(instruction.SecondQuarter, true) +
					", V" + IS_NUM(instruction.ThirdQuarter, true);
				break;


			case AND_VX_VY:
				return "AND V" + IS_NUM(instruction.SecondQuarter, true) +
					", V" + IS_NUM(instruction.ThirdQuarter, true);
				break;


			case XOR_VX_VY:
				return "XOR V" + IS_NUM(instruction.SecondQuarter, true) +
					", V" + IS_NUM(instruction.ThirdQuarter, true);
				break;


			case ADD_VX_VY:
				return "ADD V" + IS_NUM(instruction.SecondQuarter, true) +
					", V" + IS_NUM(instruction.ThirdQuarter, true);
				break;


			case SUB_VX_VY:
				return "SUB V" + IS_NUM(instruction.SecondQuarter, true) +
					", V" + IS_NUM(instruction.ThirdQuarter, true);
				break;


			case SHR_VX:
				return "SHR V" + IS_NUM(instruction.SecondQuarter, true) +
					", V" + IS_NUM(instruction.ThirdQuarter, true);
				break;


			case SUBN_VX_VY:
				return "SUBN " + IS_NUM(instruction.SecondQuarter, true) +
					", V" + IS_NUM(instruction.ThirdQuarter, true);
				break;


			case SHL_VX:
				return "SHL V" + IS_NUM(instruction.SecondQuarter, true) +
					", V" + IS_NUM(instruction.ThirdQuarter, true);
				break;


			default:
				return "[ERROR] Unknown instruction: " + IS_NUM(getFullInstruction(static_cast<int>(instruction.FirstQuarter),
					static_cast<int>(instruction.SecondQuarter),
					static_cast<int>(instruction.ThirdQuarter),
					static_cast<int>(instruction.FourthQuarter)), false);
				break;
			}

			break;

		case SNE_VX_VY:
			return "SNE V" + IS_NUM(instruction.SecondQuarter, true) +
				", V" + IS_NUM(instruction.ThirdQuarter, true);
			break;


		case LD_I_NNN:
			loadNumberToBuffer(buffer, instruction.SecondQuarter, instruction.ThirdQuarter, instruction.FourthQuarter);
			return "LD I, " + IS_NUM(buffer, false);
			break;


		case JMP_V0_NNN:
			loadNumberToBuffer(buffer, instruction.SecondQuarter, instruction.ThirdQuarter, instruction.FourthQuarter);
			return "LD V0, " + IS_NUM(buffer, false);
			break;


		case RND_VX_NN:
			loadNumberToBuffer(buffer, instruction.ThirdQuarter, instruction.FourthQuarter);
			return "RND V" + IS_NUM(instruction.SecondQuarter, true) +
				", " + IS_NUM(buffer, false);
			break;


		case DRW_VX_VY_N:
			return "DRW V" + IS_NUM(instruction.SecondQuarter, true) +
				", V" + IS_NUM(instruction.ThirdQuarter, true) +
				", " + IS_NUM(instruction.FourthQuarter, false);

			break;


		case KEYBOARD_OP:
			switch (instruction.FourthQuarter)
			{
			case SKP_VX:
				return "SKP V" + IS_NUM(instruction.SecondQuarter, true);

				break;

			case SKNP_VX:
				return "SKNP V" + IS_NUM(instruction.SecondQuarter, true);

				break;


			default:
				return "[ERROR] Unknown instruction: " + IS_NUM(getFullInstruction(static_cast<int>(instruction.FirstQuarter),
					static_cast<int>(instruction.SecondQuarter),
					static_cast<int>(instruction.ThirdQuarter),
					static_cast<int>(instruction.FourthQuarter)), false);
				break;
			}

			break;


		case REGISTER_OP:
			loadNumberToBuffer(buffer, instruction.ThirdQuarter, instruction.FourthQuarter);
			switch (buffer)
			{
			case LD_VX_DT:
				return "LD V" + IS_NUM(instruction.SecondQuarter, true) +
					", DT";
				break;


			case LD_VX_K:
				return "LD V" + IS_NUM(instruction.SecondQuarter, true) +
					", K";

				break;


			case LD_DT_VX:
				return "LD DT, V" + IS_NUM(instruction.SecondQuarter, true);

				break;


			case LD_ST_VX:
				return "LD ST, V" + IS_NUM(instruction.SecondQuarter, true);

				break;


			case ADD_I_VX:
				return "ADD I, V" + IS_NUM(instruction.SecondQuarter, true);

				break;


			case LD_F_VX:
				return "LD F, V" + IS_NUM(instruction.SecondQuarter, true);

				break;


			case LD_B_VX:
				return "LD B, V" + IS_NUM(instruction.SecondQuarter, true);

				break;


			case LD_I_VX:
				return "LD[I], V" + IS_NUM(instruction.SecondQuarter, true);

				break;


			case LD_VX_I:
				return "LD V" + IS_NUM(instruction.SecondQuarter, true) +
					", [I]";

				break;

			default:
				return "[ERROR] Unknown instruction: " + IS_NUM(getFullInstruction(static_cast<int>(instruction.FirstQuarter),
					static_cast<int>(instruction.SecondQuarter),
					static_cast<int>(instruction.ThirdQuarter),
					static_cast<int>(instruction.FourthQuarter)), false);
				break;
			}

			break;


		default:
			return "[ERROR] Unknown instruction: " + IS_NUM(getFullInstruction(static_cast<int>(instruction.FirstQuarter),
				static_cast<int>(instruction.SecondQuarter),
				static_cast<int>(instruction.ThirdQuarter),
				static_cast<int>(instruction.FourthQuarter)), false);
			break;
		}

		return "";
	}

}; //namespace: Disassembler