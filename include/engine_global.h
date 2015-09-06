#pragma once

#include <vector>
#include "objects/BaseGameObject.h"
#include "engine_context.h"

extern struct engine_context *active_context;
extern std::vector<BaseGameObject*> scene_root;
extern std::vector<BaseGameObject*> objects;

extern const char *game_setup_file;

