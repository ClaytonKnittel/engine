
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "shader.h"

#include <iostream>

#include "vecmath.h"
#include "input.h"
#include "camera.h"
#include "shape.h"
#include "objLoader.h"

#include "stb_image.h"

#include <vector>

using std::printf;

const GLint WIDTH = 800, HEIGHT = 800;

struct vertex {
    GLfloat x, y, z, r, g, b, a;
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

    // program shader("res/two.vs", "res/two.frag");
    program shader("res/tex.vs", "res/tex.frag");


    // GLfloat vertices[] {
    //     1.0f, 0.0f, -3.0f, 0.5f, 0.0f, 1.0f, 1.0f,
    //     0.0f, 1.0f, -3.0f, 0.6f, 0.3f, 0.0f, 1.0f,
    //     0.0f, 0.0f, -3.0f, 0.5f, 0.1f, 0.2f, 1.0f,
    //     1.5f, 0.0f, -3.5f, 0.1f, 0.0f, 0.6f, 1.0f,
    //     0.5f, 1.0f, -3.5f, 0.2f, 0.3f, 0.5f, 1.0f,
    //     0.5f, 0.0f, -3.5f, 0.1f, 0.1f, 0.4f, 1.0f
    // };

    const int vsize = 5;
    // GLfloat vertices[] {
    //     0.0f, 0.0f, -3.0f, 0.0f, 0.0f,
    //     0.0f, 1.0f, -3.0f, 0.0f, 1.0f,
    //     1.0f, 0.0f, -3.0f, 1.0f, 0.0f,
    //     0.0f, 1.0f, -3.0f, 0.0f, 1.0f,
    //     1.0f, 0.0f, -3.0f, 1.0f, 0.0f,
    //     1.0f, 1.0f, -3.0f, 1.0f, 1.0f
    // };

    // unsigned int indices[6] = {0, 1, 2, 3, 4, 5};

    std::vector<vec3> verts;
    std::vector<vec2> texs;
    loadObj("/users/claytonknittel/downloads/cars/Low_Poly_City_Cars.obj", verts, texs);
    GLfloat *vertices = new GLfloat[vsize * verts.size()];
    const unsigned int vertsize = vsize * verts.size();
    for (int i = 0; i < verts.size(); i++) {
        vertices[vsize * i] = verts[i].x;
        vertices[vsize * i + 1] = verts[i].y;
        vertices[vsize * i + 2] = verts[i].z;
        vertices[vsize * i + 3] = texs[i].x;
        vertices[vsize * i + 4] = texs[i].y;
    }

    GLfloat mins[3] = {100000.f, 100000.f, 100000.f};
    GLfloat maxs[3] = {-1000.f, -1000.f, -1000.f};
    for (int i = 0; i < vsize * verts.size(); i += vsize) {
        for (int j = 0; j < 3; j++) {
            mins[j] = vertices[i + j] < mins[j] ? vertices[i + j] : mins[j];
            maxs[j] = vertices[i + j] > maxs[j] ? vertices[i + j] : maxs[j];
        }
    }

    // printf("mins: %f, %f, %f\n", mins[0], mins[1], mins[2]);
    // printf("maxs: %f, %f, %f\n", maxs[0], maxs[1], maxs[2]);

    int twidth, theight, nrChannels;
    unsigned char *data = stbi_load("/users/claytonknittel/downloads/cars/tex/Car_12.png", &twidth, &theight, &nrChannels, 0);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        printf("Failed to load texture\n");
    }

    stbi_image_free(data);



    GLuint VBO, VAO;//, EBO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // the glVertexAttribPointer function sets the currently bound
    // GL_ARRAY_BUFFER as source buffer for this attribute (and stores
    // this setting, so afterwards you can freely bind another VBO)

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertsize, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vsize * sizeof(GLfloat), (GLvoid*) 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vsize * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));

    // glGenBuffers(1, &EBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);


    GLint projection = shader.uniformLoc("projection");
    GLint cammat = shader.uniformLoc("cam");
    GLint modelmat = shader.uniformLoc("model");

    int t = 0;
    float c[16];

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.11f, 0.0f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        t++;
        float dt = static_cast<float>(t) / 1000.f;

        loadPerspectiveProjection(projection, 70.0f, 1.0f, 1.f, 50.0f);
        cam.setMatrix(c);
        glUniformMatrix4fv(cammat, 1, GL_FALSE, c);
        const float m[16] = {1.f, 0, 0, 0, 0, 1.f, 0, 0, 0, 0, 1.f, 0, 0, 0, 0, 10.f};
        glUniformMatrix4fv(modelmat, 1, GL_FALSE, m);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertsize / vsize);
        glBindVertexArray(0);

        i.run(.01f);
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    delete [] vertices;

    return 0;
}
