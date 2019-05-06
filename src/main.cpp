#include <iostream>
#include <chrono>
#include <thread>
using namespace std;
using namespace std::chrono;

#include "draw.hpp"
#include "c8.hpp"

extern bool loop_continues;

int main(int argc, char *argv[])
{
	std::cout<<"c8emu"<<endl;
	if(argc!=2){
		cout<<"File name required"<<endl;
		return 1;
	}
	load_file(argv[1]);
	create_window();
	auto start = steady_clock::now();
	while(loop_continues){
		auto now = steady_clock::now();
		auto diff = now-start;
		auto end = now+milliseconds(16);
		if(diff>=seconds(1))
			start=now;
		
		exec_frame_cycle();
		poll_events();
		render_frame();
		
		this_thread::sleep_until(end);
	}	
	exit_emu();
	return 0;
}