#include <stdio.h>
#include <vector>

#include "jansson.h"
#include "objects/BaseGameObject.h"
#include "..\..\include\tools\engine_json.h"


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

		for (size_t i = 0; i < size; i++) {
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
