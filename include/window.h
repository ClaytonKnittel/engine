#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "camera.h"
#include "vecmath.h"
#include "screen.h"


class window {
    friend class screen;
    friend class input_handler;
public:
    window(GLint width, GLint height);
    ~window();

    bool shouldClose() const;
    void beginDraw() const;
    void endDraw() const;

    void setBGColor(float r, float g, float b);

    screen* getScreen();

private:
    vec3 bgColor;
    GLFWwindow *w;

    screen *_screen;

    GLint width, height;
};
