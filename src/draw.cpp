#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "draw.hpp"

SDL_Window *win;
SDL_Renderer *ren;
SDL_Event event;
bool loop_continues;

void create_window()
{
	SDL_SetMainReady();
	SDL_Init(SDL_INIT_EVERYTHING);
	win = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 384, 192, SDL_WINDOW_SHOWN);
	ren = SDL_CreateRenderer(win, -1, 0 );	
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

void render_frame()
{
	SDL_RenderPresent(ren);
}

void exit_emu()
{
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();	
}