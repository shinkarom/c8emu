#include <iostream>

#include "draw.hpp"

extern bool loop_continues;

char code[0x1000];

int main(int argc, char *argv[])
{
	std::cout<<"c8emu";
	create_window();
	while(loop_continues){
		poll_events();
		render_frame();
	}	
	exit_emu();
	return 0;
}