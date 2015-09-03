#include <stdio.h>
#include <string.h>

#include "SDL.h"
#include "jansson.h"

#include "engine_global.h"
#include "objects/BaseGameObject.h"


void BaseGameObject::update_dst(void)
{
	dst.x = (int) position.x;
	dst.y = (int) position.y;

	dst.w = (int) size.x;
	dst.h = (int) size.y;
}

int BaseGameObject::get_child_index(BaseGameObject *b)
{
	printf("search for BaseGameObject %p\n", b);
	for (int i = 0; i < children.size(); i++) {
		printf("child is %p\n", children[i]);
		if (children[i] == b) {
			return (i);
		}
	}

	return (-1);
}

BaseGameObject::BaseGameObject()
{
	sprite = nullptr;
	fps = 0.0f;
	sorting_layer = sorting_order = 0;
}

BaseGameObject::BaseGameObject(int _id) : BaseGameObject()
{
	id = _id;
}

BaseGameObject::BaseGameObject(int id, char *sprite_filename) : BaseGameObject(id)
{
	sprite_filenames.resize(1);
	sprite_filenames[0] = sprite_filename;

	init();
}

BaseGameObject::BaseGameObject(int id, std::vector<char*> _sprite_filenames, float _fps) : BaseGameObject(id)
{
	sprite_filenames = _sprite_filenames;
	fps = _fps;

	init();
}


BaseGameObject::~BaseGameObject()
{
	cleanup();
	delete sprite;
}

void BaseGameObject::init(void)
{
	if (sprite) {
		delete sprite;
	}
	sprite = new Sprite(sprite_filenames, fps);
	printf("init sprite: %p\n", sprite);
}

void BaseGameObject::draw(void)
{
	if (!draw_sprite || !enabled) {
		return;
	}
	sprite->draw(&dst);
}

void BaseGameObject::update(void)
{}

void BaseGameObject::add_child(BaseGameObject *b)
{
	if (get_child_index(b) == -1) {
		children.insert(children.end(), b);
	}
}

void BaseGameObject::remove_child(BaseGameObject *b)
{
	int index = get_child_index(b);
	if (index != -1) {
		children.erase(children.begin() + index);
	}
}

std::vector<BaseGameObject*> BaseGameObject::get_direct_children(void)
{
	return children;
}

 void BaseGameObject::get_all_children(std::vector<BaseGameObject*> *ret)
{
	ret->insert(ret->begin(), children.begin(), children.end());

	for (int i = 0; i < children.size(); i++) {
		children[i]->get_all_children(ret);
	}

	return;
}

/*
	place the object at a give position
*/
void BaseGameObject::move_to(Vector2 *pos)
{
	Vector2 delta = *pos - position;
	position = *pos;

	for (int i = 0; i < children.size(); i++) {
		children[i]->move_by(&delta);
	}
	update_dst();
}

void BaseGameObject::move_by(Vector2 *pos)
{
	position += *pos;

	for (int i = 0; i < children.size(); i++) {
		children[i]->move_by(pos);
	}

	update_dst();
}

Vector2 BaseGameObject::get_position(void)
{
	return position;
}

void BaseGameObject::resize_to(Vector2 *s)
{
	size = *s;
	update_dst();
}

void BaseGameObject::resize_by(Vector2 *s)
{
	size += *s;
	update_dst();
}

Vector2 BaseGameObject::get_size(void)
{
	return size;
}

void BaseGameObject::cleanup(void)
{
	for (int i = 0; i < sprite_filenames.size(); i++) {
		free(sprite_filenames[i]);
	}
	children.clear();
	sprite->cleanup();
}

json_t * BaseGameObject::serialize(void)
{
	json_t *root = json_object();
	json_t *tmp_obj;

	if (!root) {
		fprintf(stderr, "%s/%s+%d: unable to allocate json object\n", __FILE__, __FUNCTION__, __LINE__);
		return nullptr;
	}

	json_object_set_new(root, "id", json_integer(id));
	json_object_set_new(root, "fps", json_real(fps));
	json_object_set_new(root, "sorting_layer", json_integer(sorting_layer));
	json_object_set_new(root, "sorting_order", json_integer(sorting_order));
	json_object_set_new(root, "enabled", json_boolean(enabled));
	json_object_set_new(root, "draw_sprite", json_boolean(draw_sprite));

	json_object_set(root, "position", position.serialize());
	json_object_set(root, "size", size.serialize());

	// format the destination rectangle
	tmp_obj = json_object();
	if (!tmp_obj) {
		fprintf(stderr, "%s/%s+%d: unable to allocate json object\n", __FILE__, __FUNCTION__, __LINE__);
		json_decref(root);
		return nullptr;
	}
	json_object_set_new(tmp_obj, "x", json_integer(dst.x));
	json_object_set_new(tmp_obj, "y", json_integer(dst.y));
	json_object_set_new(tmp_obj, "w", json_integer(dst.w));
	json_object_set_new(tmp_obj, "h", json_integer(dst.h));
	json_object_set(root, "dst", tmp_obj);

	// format the sprite filenames
	tmp_obj = json_array();
	if (!tmp_obj) {
		fprintf(stderr, "%s/%s+%d: unable to allocate json object\n", __FILE__, __FUNCTION__, __LINE__);
		json_decref(root);
		return nullptr;
	}

	for (int i = 0; i < sprite_filenames.size(); i++) {
		json_t *entry = json_object();

		if (!entry) {
			fprintf(stderr, "%s/%s+%d: unable to allocate json object\n", __FILE__, __FUNCTION__, __LINE__);
			json_decref(tmp_obj);
			json_decref(root);
			return nullptr;
		}

		json_object_set_new(entry, "index", json_integer(i));
		json_object_set_new(entry, "file", json_string(sprite_filenames[i]));

		json_array_append(tmp_obj, entry);
	}
	json_object_set(root, "sprites", tmp_obj);

	// format the children
	tmp_obj = json_array();
	if (!tmp_obj) {
		fprintf(stderr, "%s/%s+%d: unable to allocate json object\n", __FILE__, __FUNCTION__, __LINE__);
		json_decref(root);
		return nullptr;
	}

	for (int i = 0; i < children.size(); i++) {
		json_array_append(tmp_obj, children[i]->serialize());
	}
	json_object_set(root, "children", tmp_obj);

	return root;
}

int BaseGameObject::deserialize(json_t * root)
{
	json_t *tmp_obj;
	if (!root) {
		fprintf(stderr, "%s/%s+%d: unable to allocate json object\n", __FILE__, __FUNCTION__, __LINE__);
		return (1);
	}

	id = (int) json_integer_value(json_object_get(root, "id"));
	fps = (float) json_real_value(json_object_get(root, "fps"));
	sorting_layer = (int) json_integer_value(json_object_get(root, "sorting_layer"));
	sorting_order = (int) json_integer_value(json_object_get(root, "sorting_order"));
	enabled = json_boolean_value(json_object_get(root, "enabled"));
	draw_sprite = json_boolean_value(json_object_get(root, "draw_sprite"));
	position.deserialize(json_object_get(root, "position"));
	size.deserialize(json_object_get(root, "size"));

	tmp_obj = json_object_get(root, "dst");
	dst.x = (int) json_integer_value(json_object_get(tmp_obj, "x"));
	dst.y = (int) json_integer_value(json_object_get(tmp_obj, "y"));
	dst.w = (int) json_integer_value(json_object_get(tmp_obj, "w"));
	dst.h = (int) json_integer_value(json_object_get(tmp_obj, "h"));

	// read the sprite filenames
	tmp_obj = json_object_get(root, "sprites");
	if (tmp_obj) {
		sprite_filenames.resize(json_array_size(tmp_obj));

		for (int i = 0; i < json_array_size(tmp_obj); i++) {
			json_t *entry = json_array_get(tmp_obj, i);
			int index = (int) json_integer_value(json_object_get(entry, "index"));
			json_t *file_object = json_object_get(entry, "file");
			const char *file = json_string_value(file_object);
			if (index < 0) {
				index = 0;
			}
			sprite_filenames[index] = (char*) calloc(json_string_length(file_object) + 1, sizeof(char));
			strcpy_s(sprite_filenames[index], json_string_length(file_object) + 1, file);
		}
	}

	// read the children
	tmp_obj = json_object_get(root, "children");
	if (tmp_obj) {
		children.resize(json_array_size(tmp_obj));

		for (int i = 0; i < json_array_size(tmp_obj); i++) {
			children[i] = new BaseGameObject();
			children[i]->deserialize(json_array_get(tmp_obj, i));
		}
	}

	update_dst();
	init();

	return 0;
}
