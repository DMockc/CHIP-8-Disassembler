using uByte = unsigned char;
using uWord = unsigned short;

void loadNumberToBuffer(uWord& buffer, uByte N1);
void loadNumberToBuffer(uWord& buffer, uByte N1, uByte N2);
void loadNumberToBuffer(uWord& buffer, uByte N1, uByte N2, uByte N3);
void clearBuffer(uWord& buffer);

uWord getFullInstruction(uByte i1, uByte i2, uByte i3, uByte i4);