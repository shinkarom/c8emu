#include <iostream>
using namespace std;

#include "draw.hpp"
#include "c8.hpp"

extern bool loop_continues;

int main(int argc, char *argv[])
{
	std::cout<<"c8emu"<<endl;
	load_file("BLINKY");
	create_window();
	while(loop_continues){
		poll_events();
		render_frame();
	}	
	exit_emu();
	return 0;
}