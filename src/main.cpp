
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "shader.h"

#include <iostream>

#include "window.h"
#include "vecmath.h"
#include "input.h"
#include "camera.h"
#include "shape.h"
#include "objLoader.h"

#include "stb_image.h"
#include "renderer.h"

#include <vector>

const GLint WIDTH = 800, HEIGHT = 800;

int main(int argc, char *argv[]) {

    window w(WIDTH, HEIGHT);
    w.setBGColor(.02f, .21f, .04f);

    renderer<textured_shape> r;

    // program shader("two.vs", "two.frag");
    // program shader("tex.vs", "tex.frag");

    std::vector<vec3> verts;
    std::vector<vec2> texs;
    loadObj("/users/claytonknittel/downloads/cars/Low_Poly_City_Cars.obj", verts, texs);

    texture tex("/users/claytonknittel/downloads/cars/tex/Car_12.png");
    textured_shape tobj(verts, texs, tex);
    tobj.bufferData();
    tobj.setScale(.1f);

    r.add(tobj);

    GLint projection = r.shader.uniformLoc("projection");
    GLint cammat = r.shader.uniformLoc("cam");
    GLint modelmat = r.shader.uniformLoc("model");
    GLint lightDir = r.shader.uniformLoc("lightDir");

    int t = 0;
    float c[16];

    tex.use();

    while (!w.shouldClose()) {
        w.beginDraw();
        r.shader.use();

        t++;
        float dt = static_cast<float>(t) / 1000.f;

        tobj.setModelMatrix(modelmat);

        glUniform3f(lightDir, cos(dt), sin(dt), 0.f);

        r.render(*w._screen);
        w.endDraw();
    }

    return 0;
}
