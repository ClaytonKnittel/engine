
#include "input.h"

using std::map;

void keycallback(GLFWwindow *w, int key, int scancode, int action, int mods) {
    static_cast<input_handler*>(glfwGetWindowUserPointer(w))->key_callback(w, key, scancode, action, mods);
}

input_handler::input_handler(const window &w) {
    // so we can call the object-specific implementation of
    // key_callback later on
    glfwSetWindowUserPointer(w.w, this);
    glfwSetKeyCallback(w.w, &keycallback);
}

input_handler::~input_handler() {

}

void input_handler::add(GLint key, std::function<void(float)> action) {
    keymap[key] = {0, action};
}

void input_handler::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_REPEAT)
        return;
    auto it = keymap.find(key);
    if (it != keymap.end())
        it->second.first = (action == GLFW_PRESS);
}

void input_handler::run(float dt) {
    for (auto it = keymap.begin(); it != keymap.end(); it++)
        if (it->second.first)
            it->second.second(dt);
}


