
#include "input.h"
#include <functional>

using std::map;

void keycallback(GLFWwindow *w, int key, int scancode, int action, int mods) {
    static_cast<InputHandler*>(glfwGetWindowUserPointer(w))->key_callback(w, key, scancode, action, mods);
}

InputHandler::InputHandler(GLFWwindow *window) {
    // so we can call the object-specific implementation of
    // key_callback later on
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, &keycallback);
}

InputHandler::~InputHandler() {

}

void InputHandler::add(GLint key, void (*action)()) {
    keymap[key] = action;
}

void InputHandler::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    printf("%zu\n", keymap.size());
}


