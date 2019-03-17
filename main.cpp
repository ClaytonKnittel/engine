
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "res/shader.h"

#include <iostream>

#include "math.h"
#include "input.h"

using std::printf;

const GLint WIDTH = 800, HEIGHT = 800;

struct vertex {
    GLfloat x, y, z;
    GLfloat r, g, b, a;
};

struct camera {
    GLfloat x, y, z;
    GLfloat phi, theta, psi; // euler angles

    void forward() {

    }

    static const GLfloat fvel;
    static const GLfloat bvel;
    static const GLfloat pvel;
    static const GLfloat rotvel;
    static const GLfloat thetavel;
    static const GLfloat tiltvel;
};
const GLfloat camera::fvel = 1.f;
const GLfloat camera::bvel = .7f;
const GLfloat camera::pvel = .8f;
const GLfloat camera::rotvel = .05f;
const GLfloat camera::thetavel = .05f;
const GLfloat camera::tiltvel = .05f;

camera cam;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    switch (key) {
    case GLFW_KEY_W:
        cam.forward();
        break;
    }
}

int main(int argc, char *argv[]) {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "rel", NULL, NULL);

    if (window == nullptr) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        glfwTerminate();
        return -1;
    }

    InputHandler i(window);

    glViewport(0, 0, width, height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    program shader("res/two.vs", "res/two.frag");

    vertex vertices[] {
        {1.0f, 0.0f, -3.0f, 1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, -5.0f, 1.0f, 1.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, -6.0f, 0.0f, 1.0f, 1.0f, 1.0f},
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(0);


    GLint projection = glGetUniformLocation(shader.programLoc(), "projection");
    GLint cam = glGetUniformLocation(shader.programLoc(), "cam");
    GLint model = glGetUniformLocation(shader.programLoc(), "model");

    int t = 0;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.3f, 0.2f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        t++;
        float dt = static_cast<float>(t) / 1000.f;

        loadPerspectiveProjection(projection, 1.0f, 1.0f, .5f, 5.0f);
        const float c[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, -dt, 1};
        glUniformMatrix4fv(cam, 1, GL_FALSE, c);
        const float m[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        glUniformMatrix4fv(model, 1, GL_FALSE, m);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}
