#include <vector>
#include <cstdint>
#include <string>
#include <fstream>
using namespace std;

#include "c8.hpp"
#include "draw.hpp"

vector <uint8_t> code(0x1000,'\0');

uint8_t registers[16],SP,DT,ST;
uint16_t stack[16],PC,I;

void init_vm()
{
	SP = 0;
	DT = 0;
	ST = 0;
	I = 0;
	PC = 0x200;
	for(uint8_t x = 0;x<16;x++){
		registers[x] = 0;
		stack[x] = 0;
	}
	draw_byte(10,10,0b11111111);
}

void load_file(string filename)
{
	ifstream ifs(filename,ifstream::in|ifstream::binary); 
	ifs.seekg(0, ios::end);
	streampos length(ifs.tellg());
	if (length) {
		ifs.seekg(0, ios::beg);
		ifs.read((char*)&code[0x200], length);
	}
	init_vm();
}