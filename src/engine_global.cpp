#include <vector>
#include <engine_context.h>
#include <objects/BaseGameObject.h>

struct engine_context *active_context;
std::vector<BaseGameObject*> scene_root;
std::vector<BaseGameObject*> objects;

const char *game_setup_file = "game_setup.json";