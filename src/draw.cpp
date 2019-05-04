#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <cstdint>
using namespace std;

#include "draw.hpp"

const uint8_t multiplier = 6;

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
	SDL_SetRenderDrawColor(ren,40,40,40,SDL_ALPHA_OPAQUE);	
	SDL_RenderClear(ren);
	SDL_SetRenderDrawColor(ren,51,255,0,SDL_ALPHA_OPAQUE);
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
	
	uint8_t SCREEN_TICKS_PER_FRAME = 1000/60;
	int frameTicks = SDL_GetTicks()-last_ticks; 
	last_ticks = SDL_GetTicks();
	if( frameTicks < SCREEN_TICKS_PER_FRAME ) {
		SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks ); 
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
		uint8_t bt = byte & 1;
		display[(x+z)%64][y]^=bt;
		byte>>=1;
	}	
}