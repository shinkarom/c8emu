#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <cstdint>
#include <ctime>
#include <cstdio>
#include <chrono>
#include <thread>
using namespace std;
using namespace std::chrono;

#include "draw.hpp"

const uint8_t multiplier = 10;
const uint16_t TICKS_PER_FRAME = 100 / 60;
auto start = steady_clock::now();

SDL_Window *win;
SDL_Renderer *ren;
SDL_Event event;
bool loop_continues;
uint8_t last_ticks;
uint8_t display[64][32] = {0};

void create_window()
{
	SDL_SetMainReady();
	SDL_Init(SDL_INIT_EVERYTHING);
	win = SDL_CreateWindow("c8emu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 64*multiplier, 32*multiplier, SDL_WINDOW_SHOWN);
	ren = SDL_CreateRenderer(win, -1,SDL_RENDERER_ACCELERATED);	
	loop_continues = true;
	last_ticks = SDL_GetTicks();
	srand(time(NULL));
}

void poll_events()
{
	if(SDL_PollEvent(&event)){
		if(event.type==SDL_QUIT){
			loop_continues = false;
		}
	}
}

void render_frame()
{
	auto now = steady_clock::now();
	auto diff = now-start;
	auto end = now+milliseconds(16);
	if(diff>=seconds(1))
		start=now;
	
	SDL_Rect rect;
	rect.w = multiplier;
	rect.h = multiplier;	
	SDL_SetRenderDrawColor(ren,0,0,0,SDL_ALPHA_OPAQUE);	
	SDL_RenderClear(ren);
	SDL_SetRenderDrawColor(ren,255,255,255,SDL_ALPHA_OPAQUE);
	for(uint8_t y = 0;y<32;y++){
		for(uint8_t x = 0;x<64;x++){
			if(display[x][y]){
				rect.x = x*multiplier;
				rect.y = y*multiplier;
				SDL_RenderFillRect(ren,&rect);				
			}
		}
	}
	
	SDL_RenderPresent(ren);	
	this_thread::sleep_until(end);
}

void exit_emu()
{
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();	
}

void draw_byte(uint8_t x,uint8_t y,uint8_t byte)
{
	for(uint8_t z = 0;z<16;z++){
		display[(x+7-z)%64][y]^= (byte & 1);
		byte>>=1;
	}	
}

void clear_display()
{
	for(uint8_t x = 0;x<8;x++){
		for(uint8_t y = 0;y<8;y++){
			display[x][y] = 0;
	}
	}
}

uint8_t get_random(uint8_t num)
{
	return (rand()%256)&num;
}

bool is_key_pressed(uint8_t keynum)
{
	const uint8_t *state = SDL_GetKeyboardState(NULL);
	const SDL_Scancode keys[16] = {SDL_SCANCODE_X,SDL_SCANCODE_1,SDL_SCANCODE_2,SDL_SCANCODE_3,SDL_SCANCODE_Q,SDL_SCANCODE_W,SDL_SCANCODE_E,SDL_SCANCODE_A,SDL_SCANCODE_S,SDL_SCANCODE_D,SDL_SCANCODE_Z,SDL_SCANCODE_C,SDL_SCANCODE_4,SDL_SCANCODE_R,SDL_SCANCODE_F,SDL_SCANCODE_W};
	return state[keys[keynum]];
}