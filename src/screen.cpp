
#include "screen.h"
#include <iostream>


screen::screen(window *parent) {
    cam.setInputs(*parent->input);
}

void screen::loadProjection(GLuint projectionMatrix) {
    loadPerspectiveProjection(projectionMatrix, fov, aspectRatio, 1.f, viewDistance);
}


window::window(GLint width, GLint height) : width(width), height(height) {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    w = glfwCreateWindow(width, height, "rel", NULL, NULL);

    if (w == nullptr) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return;
    }

    input = new input_handler(w);
    _screen = new screen(this);

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
}

window::~window() {
    delete _screen;
    delete input;
    glfwTerminate();
}

bool window::shouldClose() const {
    return glfwWindowShouldClose(w);
}

void window::beginDraw() const {
    glfwPollEvents();
    glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _screen->loadProjection();
    GLfloat c[16];
    _screen->cam.setMatrix(c);
    glUniformMatrix4fv(cammat, 1, GL_FALSE, c);
}

void window::endDraw() const {
    input->run(.01f);
    glfwSwapBuffers(w);
}

void window::setBGColor(float r, float g, float b) {
    bgColor = {r, g, b};
}
