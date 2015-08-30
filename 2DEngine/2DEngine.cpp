// 2D_engine.cpp : Defines the entry point for the console application.
//



#include <iostream>
#include <stdio.h>
#include "SDL.h"
#include "SDL_image.h"

static int init()
{
	int image_flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return (1);
	}


	if (IMG_Init(image_flags) != image_flags) {
		printf("IMG_Init Error: %s\n", IMG_GetError());
		return (1);
	}

	return (0);
}

int main(int argc, char *argv[])
{
	int ret = 0;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *bitmapSurface;
	SDL_Texture *bitmapTexture;

	// initialize subsystems
	ret = init();
	if (ret) {
		ret = 1;
		goto main_exit;
	}

	// create a window (maybe later configurable for fullscreen, ... see SDL_WindowFlags
	window = SDL_CreateWindow("2DEngine", 50, 50, 800, 450, SDL_WINDOW_SHOWN);
	if (!window) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		ret = 1;
		goto main_exit;
	}

	// create a renderer that will draw to the window, -1 specifies that we want to load whichever
	// video driver supports the flags we're passing
	// Flags: SDL_RENDERER_ACCELERATED: We want to use hardware accelerated rendering
	// SDL_RENDERER_PRESENTVSYNC: We want the renderer's present function (update screen) to be
	// synchornized with the monitor's refresh rate
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		ret = 1;
		goto main_exit;
	}

	// SDL 2.0 now uses textures to draw things but SDL_LoadBMP returns a surface
	// this lets us choose when to upload or remove textures from the GPU
	//bitmap_surface = SDL_LoadBMP("hello.bmp");
	bitmapSurface = IMG_Load("hello.tif");
	if (!bitmapSurface) {
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		ret = 1;
		goto main_exit;
	}

	// create a texture from the created surface
	bitmapTexture = SDL_CreateTextureFromSurface(renderer, bitmapSurface);
	if (!bitmapTexture) {
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		ret = 1;
		goto main_exit;
	}

	// after we created the texture we no longer need the bitmap_surface
	SDL_FreeSurface(bitmapSurface);

	// render loop
	for (;;) {
		SDL_Event event;
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				std::cout << "bye bye" << std::endl;
				break;
			}
		}

		// clear the renderer
		SDL_RenderClear(renderer);

		// draw the texture
		SDL_RenderCopy(renderer, bitmapTexture, NULL, NULL);

		// update the screen
		SDL_RenderPresent(renderer);
	}

main_exit:

	//Clean up our objects and quit
	SDL_DestroyTexture(bitmapTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return (ret);
}

