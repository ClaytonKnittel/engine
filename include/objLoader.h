#pragma once

#include <vector>
using std::vector;

#include "shape.h"

void loadObj(const char* obj_file, vector<vec3> &vertices, vector<vec2> &texCoords);
