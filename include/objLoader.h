#pragma once

#include <vector>
using std::vector;

#include "vecmath.h"

void loadObj(const char* obj_file, vector<vec3> &vertices, vector<vec2> &texCoords);
