#pragma once

#include <inttypes.h>
#include "SDL.h"

struct engine_config;

struct engine_context
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	struct engine_config *cfg;
};
