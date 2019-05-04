#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "draw.hpp"
#include <cstdint>
using namespace std;

SDL_Window *win;
SDL_Renderer *ren;
SDL_Event event;
bool loop_continues;

uint8_t display[8*8] = {0};

void create_window()
{
	SDL_SetMainReady();
	SDL_Init(SDL_INIT_EVERYTHING);
	win = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 384, 192, SDL_WINDOW_SHOWN);
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);	
	loop_continues = true;
}

void poll_events()
{
	if(SDL_PollEvent(&event)){
		if(event.type==SDL_QUIT){
			loop_continues = false;
		}
	}
}

void render_pixel(uint8_t x, uint8_t y)
{
	SDL_SetRenderDrawColor(ren,51,255,0,SDL_ALPHA_OPAQUE);
	SDL_Rect rect;
	rect.x = x*6;
	rect.y = y*6;
	rect.w = 6;
	rect.h = 6;
	SDL_RenderFillRect(ren,&rect);
}

//TODO: Draw every pixel
void render_display()
{
	SDL_SetRenderDrawColor(ren,40,40,40,SDL_ALPHA_OPAQUE);	
	SDL_RenderClear(ren);
	render_pixel(10,10);
}

void render_frame()
{
	render_display();
	SDL_RenderPresent(ren);
}

void exit_emu()
{
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();	
}

//TODO: Draw between display bytes
void draw_byte(uint8_t x,uint8_t y,uint8_t byte)
{
	uint8_t xx1 = x / 8;
	uint8_t xpos_1 = x % 8;
	if(!xpos_1)
	{
		
	}
	uint8_t yy = y / 8;
	display[(yy*8)+xx1]^=byte;
}