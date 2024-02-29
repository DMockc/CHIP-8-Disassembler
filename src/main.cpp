#include "fileReader.h"
#include "Disassembler.h"

//TODO: Arreglar bugs al cargar las instrucciones

int main(int argc, char** argv)
{
	argc = 2;
	const char* xd = "";

	if (argc != 2)
	{
		std::cout << "Usage: ./disassembler.exe <filename>.ch8";
		exit(0);
	}

	checkExtension(xd);

	std::ifstream file(xd, std::ios::binary | std::ios::ate);

	std::pair<char*, size_t> fileContent = loadFileInBuffer(file);

	

	Disassembler::disassembly(fileContent.first, fileContent.second);
	file.close();

	delete[] fileContent.first;

	return 0;
}