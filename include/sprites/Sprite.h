#pragma once

#include <vector>

#include "SDL.h"

class Sprite
{
private:
	int frames;
	float frame_timeout_value;
	float frame_timeout;
	int texture_index;

public:
	std::vector<SDL_Texture*> textures;
	Sprite();
	Sprite(char * sprite_filename);
	Sprite(std::vector<char*> sprite_filenames, float fps);
	Sprite(const Sprite&);

	~Sprite();

	void draw(SDL_Rect *dst);
	void cleanup(void);
};

