
#include <map>
#include <functional>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

class InputHandler {
public:
    InputHandler(GLFWwindow *window);
    ~InputHandler();
    void add(GLint key, std::function<void(float)>);

    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void run(float dt);
private:
    std::map<GLint, std::pair<bool, std::function<void(float)>>> keymap;
};
