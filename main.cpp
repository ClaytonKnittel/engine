
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "res/shader.h"

#include <iostream>

#include "math.h"
#include "input.h"
#include "camera.h"

using std::printf;

const GLint WIDTH = 800, HEIGHT = 800;

struct vertex {
    GLfloat x, y, z;
    GLfloat r, g, b, a;
};

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
    camera cam;
    cam.setInputs(i);

    glViewport(0, 0, width, height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    program shader("res/two.vs", "res/two.frag");

    vertex vertices[] {
        {1.0f, 0.0f, -3.0f, 0.5f, 0.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, -3.0f, 0.6f, 0.3f, 0.0f, 1.0f},
        {0.0f, 0.0f, -3.0f, 0.5f, 0.1f, 0.2f, 1.0f},
        {1.5f, 0.0f, -3.5f, 0.1f, 0.0f, 0.6f, 1.0f},
        {0.5f, 1.0f, -3.5f, 0.2f, 0.3f, 0.5f, 1.0f},
        {0.5f, 0.0f, -3.5f, 0.1f, 0.1f, 0.4f, 1.0f}
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // the glVertexAttribPointer function sets the currently bound
    // GL_ARRAY_BUFFER as source buffer for this attribute (and stores
    // this setting, so afterwards you can freely bind another VBO)

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*) 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*) (3 * sizeof(GLfloat)));

    glBindVertexArray(0);


    GLint projection = shader.uniformLoc("projection");
    GLint cammat = shader.uniformLoc("cam");
    GLint modelmat = shader.uniformLoc("model");

    int t = 0;
    float c[16];

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.11f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        t++;
        float dt = static_cast<float>(t) / 1000.f;

        loadPerspectiveProjection(projection, 1.0f, 1.0f, 1.f, 5.0f);
        cam.setMatrix(c);
        glUniformMatrix4fv(cammat, 1, GL_FALSE, c);
        const float m[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        glUniformMatrix4fv(modelmat, 1, GL_FALSE, m);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vertex));
        glBindVertexArray(0);

        i.run(.01f);
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}
