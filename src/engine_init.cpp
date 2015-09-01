
#include <stdio.h>

#include "SDL.h"
#include "SDL_image.h"

#include "engine_config.h"
#include "engine_context.h"
#include "engine_init.h"

int engine_init(struct engine_context *context)
{
	int image_flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;

	if (SDL_Init(context->cfg->subsystem_flags) != 0) {
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return (1);
	}


	if (IMG_Init(image_flags) != image_flags) {
		printf("IMG_Init Error: %s\n", IMG_GetError());
		return (1);
	}

	// create a window (maybe later configurable for fullscreen, ... see SDL_WindowFlags
	context->window = SDL_CreateWindow("2DEngine", context->cfg->screen_position_x, context->cfg->screen_position_y, context->cfg->window_width, context->cfg->window_height, context->cfg->window_flags);
	if (!context->window) {
		printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
		goto engine_init_error;
	}

	// create a renderer that will draw to the window, -1 specifies that we want to load whichever
	// video driver supports the flags we're passing
	// Flags: SDL_RENDERER_ACCELERATED: We want to use hardware accelerated rendering
	// SDL_RENDERER_PRESENTVSYNC: We want the renderer's present function (update screen) to be
	// synchornized with the monitor's refresh rate
	context->renderer = SDL_CreateRenderer(context->window, -1, context->cfg->renderer_flags);
	if (!context->renderer) {
		printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
		goto engine_init_error;
	}


	return (0);

engine_init_error:
	SDL_DestroyRenderer(context->renderer);
	SDL_DestroyWindow(context->window);

	context->window = NULL;
	context->renderer = NULL;

	return (1);
}
