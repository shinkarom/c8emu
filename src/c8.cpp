#include <vector>
#include <cstdint>
#include <string>
#include <fstream>
#include <cstdio>
#include <exception>
using namespace std;

#include "c8.hpp"
#include "draw.hpp"

vector <uint8_t> code(0x1000,'\0');

uint8_t ROM[16*5] = {0xF0,0x90,0x90,0x90,0xF0,  0x20,0x60,0x20,0x20,0x70,  0xF0,0x10,0xF0,0x80,0xF0,  0xF0,0x10,0xF0,0x10,0xF0,  0x90,0x90,0xF0,0x10,0x10, 0xF0,0x80,0xF0,0x10,0xF0,  0xF0,0x80,0xF0,0x90,0xF0,  0xF0,0x10,0x20,0x40,0x40, 0xF0,0x90,0xF0,0x90,0xF0,  0xF0,0x90,0xF0,0x10,0xF0,  0xF0,0x90,0xF0,0x90,0x90,  0xE0,0x90,0xE0,0x90,0xE0,  0xF0,0x80,0x80,0x80,0xF0,  0xE0,0x90,0x90,0x90,0xE0, 0xF0,0x80,0xF0,0x80,0xF0,  0xF0,0x80,0xF0,0x80,0x80};

uint8_t registers[16],SP,DT,ST;
uint16_t stack[16],PC,I;

void init_vm()
{
	SP = 1;
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
	if(instr==0x00E0){
		clear_display();
		//printf("CLS\n");
	} else
	if(instr==0x00EE){
		PC = stack[SP];
		SP--;
	//	printf("RET\n");
	} else		
	if((instr&0xF000)==0x1000){
		PC = instr&0x0FFF;
	//	printf("JP %d\n",PC);
	} else
	if((instr&0xF000)==0x2000){
		SP++;
		stack[SP] = PC;
		PC = instr&0x0FFF;	
	//	printf("CALL %d\n",PC);
	} else
	if((instr&0xF000)==0x3000){
		uint8_t byte = instr&0xFF;
		uint8_t x = (instr>>8)&0xF;		
		if(registers[x]==byte)
			PC+=2;
	//	printf("SE V%d, %d\n",x,byte);
	} else
	if((instr&0xF000)==0x4000){
		uint8_t byte = instr&0xFF;
		uint8_t x = (instr>>8)&0xF;		
		if(registers[x]!=byte)
			PC+=2;
	//	printf("SNE V%d, %d\n",x,byte);
	} else	
	if((instr&0xF000)==0x6000){
		uint8_t byte = instr&0xFF;
		uint8_t x = (instr>>8)&0xF;
		registers[x] = byte;
		printf("LD V%d, %d\n",x,byte);
	} else
	if((instr&0xF000)==0x7000){
		uint8_t byte = instr&0xFF;
		uint8_t x = (instr>>8)&0xF;
		registers[x] += byte;
	//	printf("ADD V%d, %d\n",x,byte);
	} else	
	if((instr&0xF00F)==0x8000){
		uint8_t y = (instr>>4)&0xF;
		uint8_t x = (instr>>8)&0xF;
		registers[x] = registers[y];
		printf("LD V%d, V%d\n",x,y);
	} else	
	if((instr&0xF00F)==0x8001){
		uint8_t y = (instr>>4)&0xF;
		uint8_t x = (instr>>8)&0xF;
		registers[x] = registers[y];
	//	printf("OR V%d, V%d\n",x,y);
	} else		
	if((instr&0xF00F)==0x8002){
		uint8_t y = (instr>>4)&0xF;
		uint8_t x = (instr>>8)&0xF;
		registers[x] &= registers[y];
	//	printf("AND V%d, V%d\n",x,y);
	} else		
	if((instr&0xF00F)==0x8003){
		uint8_t y = (instr>>4)&0xF;
		uint8_t x = (instr>>8)&0xF;
		registers[x] ^= registers[y];
	//	printf("XOR V%d, V%d\n",x,y);
	} else	
	if((instr&0xF00F)==0x8004){
		uint8_t y = (instr>>4)&0xF;
		uint8_t x = (instr>>8)&0xF;
		if(registers[x] + registers[y]>255){
			registers[x]=registers[x] + registers[y]-255;
			registers[15] = 1;
		} else{
			registers[x]=registers[x] + registers[y];
			registers[15]=0;
		}			
	//	printf("ADD V%d, V%d\n",x,y);
	} else	
	if((instr&0xF00F)==0x8005){
		uint8_t y = (instr>>4)&0xF;
		uint8_t x = (instr>>8)&0xF;
		registers[15] =(registers[x]>registers[y]) ? 1 : 0;
		registers[x]-=registers[y];
	//	printf("SUB V%d, V%d\n",x,y);
	} else		
	if((instr&0xF00F)==0x8006){
		uint8_t x = (instr>>8)&0xF;
		uint8_t y = (instr>>4)&0xF;
		registers[15] = ((registers[y]&1)==1)?1: 0;
		registers[x]=registers[y]>>1;
	//	printf("SHR V%d\n",x);
	} else			
	if((instr&0xF00F)==0x8007){
		uint8_t y = (instr>>4)&0xF;
		uint8_t x = (instr>>8)&0xF;
		if(registers[y] > registers[x]){
			registers[15] = 1;
		} 
		registers[x]-=registers[y];
	//	printf("SUBN V%d, V%d\n",x,y);
	} else		
	if((instr&0xF00F)==0x8007){
		uint8_t x = (instr>>8)&0xF;
		uint8_t y = (instr>>4)&0xF;
		registers[15] = ((registers[y]&0x10000000)==0x10000000)?1: 0;
		registers[x]=registers[y]<<1;
	//	printf("SHL V%d\n",x);
	} else			
	if((instr&0xF000)==0xA000){
		I = instr&0x0FFF;
	//	printf("LD I, %d\n",I);
	} else
	if((instr&0xF000)==0xC000){
		uint8_t byte = instr&0xFF;
		uint8_t x = (instr>>8)&0xF;
		registers[x] = get_random(byte);
	//	printf("RND V%d, %d\n",x,byte);
	} else 
	if((instr&0xF000)==0xD000){
		uint8_t n = instr&0xF;
		if(n==0)n=16;
		uint8_t y = (instr>>4)&0xF;
		uint8_t x = (instr>>8)&0xF;
		for(uint8_t z = 0;z<=n;z++){
			draw_byte(registers[x],(registers[y]+z)%32,code[I+z]);
		}
		printf("DRW V%d (%d),V%d (%d),%d\n",x,registers[x],y,registers[y],n);
	} else
	if((instr&0xF0FF)==0xF007){
		uint8_t x = (instr>>8)&0xF;
		registers[x]=DT;
	//	printf("LD V%d, DT (%d)\n",x,DT);
	} else			
	if((instr&0xF0FF)==0xF015){
		uint8_t x = (instr>>8)&0xF;
		DT=registers[x];
	//	printf("LD DT, V%d (%d)\n",x,DT);
	} else	
	if((instr&0xF0FF)==0xF018){
		uint8_t x = (instr>>8)&0xF;
		ST=registers[x];
	//	printf("LD ST, V%d (%d)\n",x,ST);
	} else			
	if((instr&0xF0FF)==0xF01E){
		uint8_t x = (instr>>8)&0xF;
		I=I+registers[x];
	//	printf("ADD I, V%d\n",x);
	} else
	if((instr&0xF0FF)==0xF029){
		uint8_t x = (instr>>8)&0xF;
		I=(x*5);
	//	printf("LD F, V%d\n",x);
	} else		
	if((instr&0xF0FF)==0xF033){
		uint8_t x = (instr>>8)&0xF;
		uint8_t n = registers[x];
		code[I] = n/100;
		n/=100;
		code[I+1] = n/10;
		n/=10;
		code[I+2] = n;
	//	printf("LD B, V%d\n",x);
	} else		
	if((instr&0xF0FF)==0xF055){
		uint8_t x = (instr>>8)&0xF;
		for(uint8_t z=0;z<=x;z++)
			code[I+z] = registers[z];
		I+=x;
	//	printf("LD [I], V%d\n",x);
	} else		
	if((instr&0xF0FF)==0xF065){
		uint8_t x = (instr>>8)&0xF;
		for(uint8_t z=0;z<=x;z++)
			registers[z] = code[I+z];
		I+=x;
	//	printf("LD V%d, [I]\n",x);
	} else
		
	printf("%d %d %d\n",PC,instr>>8,instr&0xFF);
}

void exec_frame_cycle()
{
	if(DT>0) DT--;
	if(ST>0) ST--;
	for(uint8_t n = 0;n<=16;n++){
		uint8_t hi = code[PC],lo=code[PC+1];
		uint16_t hl = (hi<<8)|lo;	
		PC+=2;			
		exec_instruction(hl);
	}
}

void load_file(string filename)
{	
	memcpy(&code[0],&ROM[0],16*5);
	ifstream ifs(filename,ifstream::in|ifstream::binary); 
	ifs.seekg(0, ios::end);
	streampos length(ifs.tellg());
	if (length) {
		ifs.seekg(0, ios::beg);
		ifs.read((char*)&code[0x200], length);
	}
	init_vm();
}