#include <iostream>
#include <SDL2/SDL.h>

SDL_Window *win;
SDL_Renderer *ren;

int main(int argc, char *argv[])
{
	std::cout<<"c8emu";
	SDL_Init(SDL_INIT_VIDEO);
	win = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 384, 192, SDL_WINDOW_SHOWN);
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_RenderPresent(ren);
	SDL_Delay(5000);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}