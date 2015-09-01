#pragma once

#include <inttypes.h>

struct engine_config
{
	int screen_position_x;
	int screen_position_y;
	int window_width;
	int window_height;

	uint32_t subsystem_flags;
	uint32_t window_flags;
	uint32_t renderer_flags;

};
