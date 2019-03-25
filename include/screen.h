#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "window.h"

class screen {
    friend class window;

public:

    void loadProjection(GLuint projectionMatrix);
    void loadCameraMatrix(GLuint camMatrix);
private:
    screen(GLfloat fov, GLfloat aspectRatio, GLfloat viewDistance);

    camera cam;

    GLint width, height;
    GLfloat fov, aspectRatio, viewDistance;
};