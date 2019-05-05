#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <cstdint>
#include <ctime>
using namespace std;

#include "draw.hpp"

const uint8_t multiplier = 10;

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
	
	int frameTicks = SDL_GetTicks()-last_ticks; 
	last_ticks = SDL_GetTicks();
	if(frameTicks < 17) {
		SDL_Delay(17 - frameTicks); 
	}
}

void exit_emu()
{
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();	
}

void draw_byte(uint8_t x,uint8_t y,uint8_t byte)
{
	for(uint8_t z = 0;z<8;z++){
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