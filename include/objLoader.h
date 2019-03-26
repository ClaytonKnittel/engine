#pragma once

#include <vector>
using std::vector;

#include "vecmath.h"

const char* get_texture_loc(const char* obj_file);

void loadObj(const char* obj_file, vector<vec3> &vertices, vector<vec2> &texCoords);
void loadObj(const char* obj_file, vector<vec3> &vertices, vector<vec3> &normals);
void loadObj(const char* obj_file, vector<vec3> &vertices, vector<vec3> &normals, vector<vec2> &texCoords);
