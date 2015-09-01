#include <iostream>
#include <vector>

#include "SDL.h"
#include "SDL_image.h"
#include "engine_global.h"
#include "engine_time.h"
#include "engine_render_functions.h"
#include "sprites/Sprite.h"


Sprite::Sprite()
{}

Sprite::Sprite(const char *sprite_filename)
{
	frame_timeout = frame_timeout_value = FLT_MAX;

	frames = 1;

	textures.resize(1);
	textures[0] = IMG_LoadTexture(active_context->renderer, sprite_filename);
	printf("b: %p\n", textures[0]);

	texture_index = 0;

	if (!textures[0]) {
		std::cout << "unable to load texture '" << sprite_filename << "': " << SDL_GetError() << std::endl;
		cleanup();
		return;
	}
}

Sprite::Sprite(std::vector<const char *> sprite_filenames, float fps)
{
	int i;
	if (fps > 0) {
		frame_timeout = frame_timeout_value = 1.0f / fps;
	} else if (fps < 0) {
		frame_timeout = frame_timeout_value = -1.0f / fps;
	} else {
		frame_timeout = frame_timeout_value = FLT_MAX;
	}

	frames = (int) sprite_filenames.size();

	textures.resize(frames);
	texture_index = 0;

	for (i = 0; i < frames; i++) {
		textures[i] = NULL;
	}

	for (i = 0; i < frames; i++) {
		textures[i] = IMG_LoadTexture(active_context->renderer, sprite_filenames[i]);

		if (!textures[i]) {
			std::cout << "unable to load texture '" << sprite_filenames[i] << "': " << SDL_GetError() << std::endl;
			cleanup();
			return;
		}
	}

}

Sprite::Sprite(const Sprite& s)
{
	frames = s.frames;
	frame_timeout = s.frame_timeout;
	frame_timeout_value = s.frame_timeout_value;

	textures.resize(s.textures.size());
	for (size_t i = 0; i < s.textures.size(); i++) {
		textures[i] = s.textures[i];
	}
	texture_index = s.texture_index;
}

Sprite::~Sprite()
{
	std::cout << "sprite destroy" << std::endl;
	cleanup();
}

void Sprite::cleanup(void)
{
	for (int i = 0; i < frames; i++) {
		if (textures[i]) {
			SDL_DestroyTexture(textures[i]);
		}
	}

	frame_timeout = frame_timeout_value = 0;
	frames = 0;
	textures.clear();
	texture_index = 0;
}

void Sprite::draw(SDL_Rect *dst)
{
	if (!frames) {
		return;
	} else if (frames > 1) {
		frame_timeout -= Time.delta_time;

		if (frame_timeout < 0) {
			frame_timeout = frame_timeout_value;

			if (texture_index == frames - 1) {
				texture_index = 0;
			} else {
				texture_index++; 
			}
		}
	}
	render_texture(active_context->renderer, textures[0], dst);
}
