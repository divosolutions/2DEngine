// 2D_engine.cpp : Defines the entry point for the console application.
//



#include <iostream>
#include <SDL.h>

int main(int, char**) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	else {
		std::cout << "everthing is alright" << std::endl;
	}
	SDL_Quit();
	return 0;
}
