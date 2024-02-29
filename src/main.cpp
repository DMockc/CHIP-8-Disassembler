#include "Disassembler.h"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: ./disassembler.exe <filename>.ch8\n";
		exit(-1);
	}

	if (!checkExtension(argv[1]))
	{
		std::cerr << " File extension isn't correct!\n";
		exit(-1);
	}

	std::ifstream file(argv[1], std::ios::binary | std::ios::ate);

	std::pair<char*, size_t> fileContent = loadFileInBuffer(file);

	Disassembler::disassembly(fileContent.first, fileContent.second);

	file.close();

	delete[] fileContent.first;

	std::cout << "The file has been successfully disassembled!\n";

	return 0;
}