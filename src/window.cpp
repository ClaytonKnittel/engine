
#include "window.h"
#include <iostream>


window::window(GLint width, GLint height) : width(width), height(height) {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    w = glfwCreateWindow(width, height, "rel", NULL, NULL);

    if (w == nullptr) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return;
    }

    // input = new input_handler(w);
    _screen = new screen(70.f, 1.f, 50.f);
    glfwGetFramebufferSize(w, &_screen->width, &_screen->height);

    glfwMakeContextCurrent(w);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        glfwTerminate();
        return;
    }

    glViewport(0, 0, _screen->width, _screen->height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_MULTISAMPLE);
}

window::~window() {
    delete _screen;
    glfwTerminate();
}

bool window::shouldClose() const {
    return glfwWindowShouldClose(w);
}

void window::beginDraw() const {
    glfwPollEvents();
    glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void window::endDraw() const {
    glfwSwapBuffers(w);
}

void window::setBGColor(float r, float g, float b) {
    bgColor = {r, g, b};
}

screen* window::getScreen() {
    return _screen;
}
