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
	uint8_t x = (instr>>8)&0xF;
	uint8_t y = (instr>>4)&0xF;
	uint8_t byte = instr&0xFF;
	if(instr==0x00E0){
		clear_display();
		printf("CLS\n");
	} else
	if(instr==0x00EE){
		PC = stack[SP];
		SP--;
		printf("RET\n");
	} else		
	if((instr&0xF000)==0x1000){
		PC = instr&0x0FFF;
		printf("JP %d\n",PC);
	} else
	if((instr&0xF000)==0x2000){
		SP++;
		stack[SP] = PC;
		PC = instr&0x0FFF;	
		printf("CALL %d\n",PC);
	} else
	if((instr&0xF000)==0x3000){
		if(registers[x]==byte)
			PC+=2;
		printf("SE V%d, %d\n",x,byte);
	} else
	if((instr&0xF000)==0x4000){
		if(registers[x]!=byte)
			PC+=2;
		printf("SNE V%d, %d\n",x,byte);
	} else	
	if((instr&0xF00F)==0x5000){	
		if(registers[x]==registers[y])
			PC+=2;
		printf("SE V%d, V%d\n",x,y);
	} else
	if((instr&0xF000)==0x6000){
		registers[x] = byte;
		printf("LD V%d, %d\n",x,byte);
	} else
	if((instr&0xF000)==0x7000){
		registers[x] += byte;
		printf("ADD V%d, %d\n",x,byte);
	} else	
	if((instr&0xF00F)==0x8000){
		registers[x] = registers[y];
		printf("LD V%d, V%d\n",x,y);
	} else	
	if((instr&0xF00F)==0x8001){
		registers[x] |= registers[y];
		printf("OR V%d, V%d\n",x,y);
	} else		
	if((instr&0xF00F)==0x8002){
		registers[x] &= registers[y];
		printf("AND V%d, V%d\n",x,y);
	} else		
	if((instr&0xF00F)==0x8003){
		registers[x] ^= registers[y];
		printf("XOR V%d, V%d\n",x,y);
	} else	
	if((instr&0xF00F)==0x8004){
		registers[15]=(registers[x] + registers[y]>255)?1:0;		
		registers[x]=registers[x] + registers[y];
		printf("ADD V%d, V%d\n",x,y);
	} else	
	if((instr&0xF00F)==0x8005){
		registers[15] =(registers[x]>registers[y]) ? 1 : 0;
		registers[x]-=registers[y];
		printf("SUB V%d, V%d\n",x,y);
	} else		
	if((instr&0xF00F)==0x8006){
		registers[15] = (registers[x]&1);
		registers[x]=registers[x]>>1;
		printf("SHR V%d\n",x);
	} else			
	if((instr&0xF00F)==0x8007){
		registers[15] = (registers[y] > registers[x])?1:0;
		registers[x]-=registers[y];
		printf("SUBN V%d, V%d\n",x,y);
	} else		
	if((instr&0xF00F)==0x800E){
		registers[15] = (registers[x]&0x10000000)>>7;;
		registers[x]=registers[x]<<1;
		printf("SHL V%d\n",x);
	} else		
	if((instr&0xF00F)==0x9000){	
		if(registers[x]!=registers[y])
			PC+=2;
		printf("SNE V%d, V%d\n",x,y);
	} else		
	if((instr&0xF000)==0xA000){
		I = instr&0x0FFF;
		printf("LD I, %d\n",I);
	} else
	if((instr&0xF000)==0xB000){
		PC = (instr&0x0FFF)+registers[0];
		printf("JP %d\n",PC);
	} else
	if((instr&0xF000)==0xC000){
		registers[x] = get_random(byte);
		printf("RND V%d, %d\n",x,byte);
	} else 
	if((instr&0xF000)==0xD000){
		uint8_t n = instr&0xF;
		if(n==0)n=16;
		for(uint8_t z = 0;z<n;z++){
			draw_byte(registers[x],(registers[y]+z)%32,code[I+z]);
		}
		printf("DRW V%d (%d),V%d (%d),%d\n",x,registers[x],y,registers[y],n);
	} else
	if((instr&0xF0FF)==0xE09E){
		if(is_key_pressed(registers[x]))
			PC+=2;
		printf("SKP V%d\n",x);
	} else
	if((instr&0xF0FF)==0xE0A1){
		if(!is_key_pressed(registers[x]))
			PC+=2;
		printf("SKNP V%d\n",x);
	} else		
	if((instr&0xF0FF)==0xF007){
		registers[x]=DT;
		printf("LD V%d, DT (%d)\n",x,DT);
	} else	
	if((instr&0xF0FF)==0xF00A){
		uint8_t key=-1;
		do{
			for(uint8_t z = 0;z<16;z++){
				if(is_key_pressed(z)){
					key = z;
				}
			}
		} while(key==-1);
		registers[x]=key;
		printf("LD V%d, K (%d) \n",x,key);
	} else			
	if((instr&0xF0FF)==0xF015){
		DT=registers[x];
		printf("LD DT, V%d (%d)\n",x,DT);
	} else	
	if((instr&0xF0FF)==0xF018){
		ST=registers[x];
		printf("LD ST, V%d (%d)\n",x,ST);
	} else			
	if((instr&0xF0FF)==0xF01E){
		I+=registers[x];
		printf("ADD I, V%d\n",x);
	} else
	if((instr&0xF0FF)==0xF029){
		I=(registers[x]*5);
		printf("LD F, V%d\n",x);
	} else		
	if((instr&0xF0FF)==0xF033){
		uint8_t n = registers[x];
		code[I] = n/100;
		n%=100;
		code[I+1] = n/10;
		code[I+2] = n % 10;
		printf("LD B, V%d (%d)\n",x,n);
	} else		
	if((instr&0xF0FF)==0xF055){
		for(uint8_t z=0;z<=x;z++)
			code[I+z] = registers[z];
		printf("LD [I], V%d\n",x);
	} else		
	if((instr&0xF0FF)==0xF065){
		for(uint8_t z=0;z<=x;z++)
			registers[z] = code[I+z];
		printf("LD V%d, [I]\n",x);
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