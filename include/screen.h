#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "camera.h"

class screen {
    friend class window;

public:

    void loadProjection(GLuint projectionMatrix) const;
    void loadCameraMatrix(GLuint camMatrix) const;

    camera &getCamera();
private:
    screen(GLfloat fov, GLfloat aspectRatio, GLfloat viewDistance);

    camera cam;

    GLint width, height;
    GLfloat fov, aspectRatio, viewDistance;
};