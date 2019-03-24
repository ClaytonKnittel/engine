#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "camera.h"
#include "input.h"
#include "vecmath.h"

class window;

class screen {
    friend class window;

public:
    screen(window *parent);

    void loadProjection();
private:
    camera cam;

    // renderer<> *rend;

    GLint projectionMatrix, cameraMatrix;
    GLint width, height;
    GLfloat fov, aspectRatio, viewDistance;
};

class window {
friend class screen;
public:
    window(GLint width, GLint height);
    ~window();

    bool shouldClose() const;
    void beginDraw() const;
    void endDraw() const;

    void setBGColor(float r, float g, float b);

private:
    vec3 bgColor;
    input_handler *input;
    GLFWwindow *w;
    screen *_screen;
    GLint width, height;
};
