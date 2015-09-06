#pragma once

#include <vector>

#include "jansson.h"

#include "sprites/Sprite.h"
#include "tools/Vector2.h"

class BaseGameObject
{
	int id;
	std::vector<char*> sprite_filenames;
	float fps;
	Vector2 position;
	SDL_Rect dst;
	Sprite *sprite;
	Vector2 size;
	std::vector<BaseGameObject*> children;

	void update_dst(void);
	int get_child_index(BaseGameObject *);
public:
	bool enabled;
	bool draw_sprite;

	int sorting_layer;
	int sorting_order;

	BaseGameObject();
	BaseGameObject(int id);
	BaseGameObject(int id, char*);
	BaseGameObject(int id, std::vector<char*>, float fps);
	~BaseGameObject();

	void init(void);
	void draw(void);
	void update(void);

	void add_child(BaseGameObject *);
	void remove_child(BaseGameObject *);
	std::vector<BaseGameObject*> get_direct_children(void);
	void BaseGameObject::get_all_children(std::vector<BaseGameObject*> *ret);

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

