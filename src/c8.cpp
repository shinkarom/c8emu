#include <vector>
#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>
#include <exception>
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
}

void exec_instruction(uint16_t instr)
{	
	if(PC>=0x1000) throw new runtime_error("Out of Bounds");
	if((instr&0xF000)==0x1000){
		PC = instr&0x0FFF;
		cout<<"jump to "<<PC<<endl;
	}
	else
		cout<<(instr>>8)<<" "<<(instr&0xFF)<<endl;
}

void exec_frame_cycle()
{
	if(DT>0) DT--;
	if(ST>0) ST--;
	for(uint8_t n = 0;n<=8;n++){
		uint8_t hi = code[PC],lo=code[PC+1];
		uint16_t hl = (hi<<8)|lo;	
		PC+=2;			
		exec_instruction(hl);
	}
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