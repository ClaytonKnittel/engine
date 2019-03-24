
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "shader.h"

#include <iostream>

#include "screen.h"
#include "vecmath.h"
#include "input.h"
#include "camera.h"
#include "shape.h"
#include "objLoader.h"

#include "stb_image.h"

#include <vector>

const GLint WIDTH = 800, HEIGHT = 800;

int main(int argc, char *argv[]) {

    window w(WIDTH, HEIGHT);

    // program shader("two.vs", "two.frag");
    program shader("tex.vs", "tex.frag");


    // GLfloat vertices[] {
    //     1.0f, 0.0f, -3.0f, 0.5f, 0.0f, 1.0f, 1.0f,
    //     0.0f, 1.0f, -3.0f, 0.6f, 0.3f, 0.0f, 1.0f,
    //     0.0f, 0.0f, -3.0f, 0.5f, 0.1f, 0.2f, 1.0f,
    //     1.5f, 0.0f, -3.5f, 0.1f, 0.0f, 0.6f, 1.0f,
    //     0.5f, 1.0f, -3.5f, 0.2f, 0.3f, 0.5f, 1.0f,
    //     0.5f, 0.0f, -3.5f, 0.1f, 0.1f, 0.4f, 1.0f
    // };

    const int vsize = 8;
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

    texture tex("/users/claytonknittel/downloads/cars/tex/Car_12.png");
    textured_shape tobj(verts, texs, tex);
    tobj.bufferData();


    GLint projection = shader.uniformLoc("projection");
    GLint cammat = shader.uniformLoc("cam");
    GLint modelmat = shader.uniformLoc("model");
    GLint lightDir = shader.uniformLoc("lightDir");

    int t = 0;
    float c[16];

    tex.use();

    while (!w.shouldClose()) {
        glfwPollEvents();
        glClearColor(0.11f, 0.0f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        t++;
        float dt = static_cast<float>(t) / 1000.f;

        loadPerspectiveProjection(projection, 70.0f, 1.0f, 1.f, 50.0f);
        w._screen->cam.setMatrix(c);
        glUniformMatrix4fv(cammat, 1, GL_FALSE, c);
        const float m[16] = {1.f, 0, 0, 0, 0, 1.f, 0, 0, 0, 0, 1.f, 0, 0, 0, 0, 10.f};
        glUniformMatrix4fv(modelmat, 1, GL_FALSE, m);
        // tobj.setModelMatrix(modelmat);

        glUniform3f(lightDir, cos(dt), sin(dt), 0.f);

        tobj.draw();

        w.input->run(.01f);
        glfwSwapBuffers(w.w);
    }

    return 0;
}
