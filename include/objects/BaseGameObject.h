#pragma once

#include <vector>

#include "jansson.h"
#include "SDL.h"

#include "sprites/Sprite.h"
#include "tools/Vector2.h"

class BaseGameObject
{
	int id;
	std::vector<const char*> sprite_filenames;
	float fps;
	Vector2 position;
	SDL_Rect dst;
	Sprite *sprite;
	Vector2 size;

	void update_dst(void);
public:
	bool enabled;
	bool draw_sprite;

	int sorting_layer;
	int sorting_order;

	BaseGameObject();
	BaseGameObject(int id);
	BaseGameObject(int id, const char*);
	BaseGameObject(int id, std::vector<const char*>, float fps);
	~BaseGameObject();

	void init(void);
	void draw(void);
	void update(void);

	void move_to(Vector2*);
	void move_by(Vector2*);
	Vector2 get_position(void);

	void resize_to(Vector2*);
	void resize_by(Vector2*);
	Vector2 get_size(void);

	void cleanup(void);

	json_t* serialize(void);
	int deserialize(json_t*);
};

