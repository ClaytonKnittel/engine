#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "camera.h"
#include "input.h"

class window;

class screen {
    friend class window;

public:
    screen(window *parent);
// private:
    camera cam;
    GLint width, height;
};

class window {
friend class screen;
public:
    window(GLint width, GLint height);
    ~window();

    bool shouldClose() const;

// private:
    input_handler *input;
    GLFWwindow *w;
    screen *_screen;
    GLint width, height;
};
