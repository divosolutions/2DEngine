#pragma once

#include <vector>

#include "sprites/Sprite.h"
#include "tools/Vector2.h"

class BaseGameObject
{
public:
	bool enabled;
	bool draw_sprite;

	Sprite *sprite;
	Vector2 position;
	Vector2 size;
	int sorting_layer;
	int sorting_order;

	BaseGameObject();
	BaseGameObject(const char*);
	BaseGameObject(std::vector<const char*>, float fps);
	~BaseGameObject();

	void draw(void);
	void cleanup(void);
};

