// 2D_engine.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <stdio.h>
#include <vector>

#include "jansson.h"
#include "SDL.h"
#include "SDL_image.h"

#include "engine_global.h"
#include "engine_time.h"
#include "engine_config.h"
#include "engine_context.h"
#include "engine_init.h"
#include "engine_render_functions.h"
#include "objects/BaseGameObject.h"

struct engine_context *active_context;
std::vector<BaseGameObject*> scene_root;
std::vector<BaseGameObject*> objects;

const char *game_setup_file = "game_setup.json";

void serialize_objects_to_file(const char *filename, std::vector<BaseGameObject*> *objects)
{
	json_t *root = json_object();
	json_t *object_array = json_array();

	if (!root) {
		fprintf(stderr, "%s/%s+%d: unable to allocate json object\n", __FILE__, __FUNCTION__, __LINE__);
		return;
	}

	for (std::vector<BaseGameObject*>::iterator it = objects->begin(); it != objects->end(); it++) {
		json_array_append(object_array, (*it)->serialize());
	}
	
	json_object_set(root, "scene", object_array);

	FILE *fp;
	fopen_s(&fp, filename, "w");

	if (!fp) {
		fprintf(stderr, "%s/%s+%d: unable to open file '%s' for writing\n", __FILE__, __FUNCTION__, __LINE__, filename);
		json_decref(root);
		return;
	}

	json_dumpf(root, fp, JSON_INDENT(4));

	fclose(fp);
	json_decref(root);
}

int deserialize_objects_from_file(const char *filename, std::vector<BaseGameObject*> *objects)
{
	json_t *root;
	json_error_t json_error_ret;

	FILE *fp;
	fopen_s(&fp, filename, "r");

	if (!fp) {
		fprintf(stderr, "%s/%s+%d: unable to open file '%s' for reading\n", __FILE__, __FUNCTION__, __LINE__, filename);
		return (1);
	}

	root = json_loadf(fp, 0, &json_error_ret);
	fclose(fp);

	if (root) {
		json_t *game_objects = json_object_get(root, "scene");
		size_t size = json_array_size(game_objects);

		objects->resize(size);

		for (int i = 0; i < size; i++) {
			(*objects)[i] = new BaseGameObject();
			(*objects)[i]->deserialize(json_array_get(game_objects, i));
		}
	} else {
		fprintf(stderr, "error while parsing json in '%s' line %d:%d: %s\n", filename, json_error_ret.line, json_error_ret.column, json_error_ret.text);
		return (1);
	}
	json_decref(root);

	return (0);
}

int main(int argc, char *argv[])
{
	int ret = 0;

	std::vector<const char*> fnames(4);
	std::vector<std::vector<BaseGameObject*>> ordered_render_objects;

	struct engine_context context;
	struct engine_config cfg;

	context.cfg = &cfg;

	cfg.screen_position_x = 50;
	cfg.screen_position_y = 50;
	cfg.window_width = 800;
	cfg.window_height = 450;

	cfg.subsystem_flags = SDL_INIT_VIDEO;
	cfg.window_flags = SDL_WINDOW_SHOWN;
	cfg.renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	
	active_context = &context;

	// initialize subsystems
	ret = engine_init(&context);
	if (ret) {
		ret = 1;
		goto main_exit;
	}

	// SDL 2.0 now uses textures to draw things but SDL_LoadBMP returns a surface
	// this lets us choose when to upload or remove textures from the GPU

	if (deserialize_objects_from_file(game_setup_file, &scene_root)) {
		goto main_exit;
	}

	for (int i = 0; i < scene_root.size(); i++) {
		objects.insert(objects.begin(), scene_root[i]);
		scene_root[i]->get_all_children(&objects);
	}

	ordered_render_objects.resize(4);

	ordered_render_objects[0].resize(1);
	ordered_render_objects[0][0] = objects[0];

	ordered_render_objects[1].resize(1);
	ordered_render_objects[1][0] = objects[1];

	ordered_render_objects[2].resize(1);
	ordered_render_objects[2][0] = objects[2];

	ordered_render_objects[3].resize(1);
	ordered_render_objects[3][0] = objects[3];

	// render loop
	bool run = true;
	engine_time_init();
	Vector2 *m = new Vector2(24.0f, 24.0f);
	
	while (run) {
		SDL_Event event;

		engine_time_update();

		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN) {
				std::cout << "bye bye" << std::endl;
				run = false;
			}
		}

		// clear the renderer
		SDL_RenderClear(active_context->renderer);

		for (int sorting_layer = 0; sorting_layer < ordered_render_objects.size(); sorting_layer++) {
			for (int sorting_order = 0; sorting_order < ordered_render_objects[sorting_layer].size(); sorting_order++) {
				ordered_render_objects[sorting_layer][sorting_order]->draw();
			}
		}

		scene_root[0]->move_by(&(*m * Time.delta_time));

		if (scene_root[0]->get_position().y > cfg.window_height / 2) {
			scene_root[0]->move_to(&Vector2(0, 0));
		}

		// present the content rendered to the renderer
		SDL_RenderPresent(active_context->renderer);
	}
	delete m;

main_exit:

	serialize_objects_to_file("out.json", &scene_root);

	//Clean up our objects and quit
	for (std::vector<BaseGameObject*>::iterator it = objects.begin(); it != objects.end(); it++) {
		delete (*it);
	}

	SDL_DestroyRenderer(context.renderer);
	SDL_DestroyWindow(context.window);
	SDL_Quit();

	return (ret);
}

