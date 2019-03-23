#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>

using std::vector;

struct vec3 {
    GLfloat x, y, z;
};

struct vec2 {
    GLfloat x, y;
};

class mesh {
public:
    mesh(vector<vec3> vertices, vector<unsigned int> indices);

private:
    GLuint vao, vbo;

    vector<vec3> vertices;
    vector<unsigned int> indices;
};


// 441 vertices
