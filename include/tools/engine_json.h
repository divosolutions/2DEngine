#pragma once

void serialize_objects_to_file(const char * filename, std::vector<BaseGameObject*>* objects);
int deserialize_objects_from_file(const char * filename, std::vector<BaseGameObject*>* objects);
