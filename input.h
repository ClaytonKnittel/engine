
#include <map>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

class InputHandler {
public:
    InputHandler(GLFWwindow *window);
    ~InputHandler();
    void add(GLint key, void (*action)());

    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
private:
    std::map<GLint, void (*)()> keymap;
};
