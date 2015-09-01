#include <stdio.h>
#include "SDL.h"

#include "objects/BaseGameObject.h"


BaseGameObject::BaseGameObject()
{
	sorting_layer = sorting_order = 0;
}

BaseGameObject::BaseGameObject(const char *sprite_filename) : BaseGameObject()
{
	printf("foo\n");
	sprite = new Sprite(sprite_filename);
	printf("bar\n");
}

BaseGameObject::BaseGameObject(std::vector<const char*> sprite_filenames, float fps) : BaseGameObject()
{
	sprite = new Sprite(sprite_filenames, fps);
}


BaseGameObject::~BaseGameObject()
{
	delete sprite;
}

void BaseGameObject::draw(void)
{
	if (!draw_sprite || !enabled) {
		return;
	}
	SDL_Rect dst = { (int) position.x, (int) position.y, (int) size.x, (int) size.y };
	sprite->draw(&dst);
}

void BaseGameObject::cleanup(void)
{
	sprite->cleanup();
}
