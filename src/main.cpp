
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <iostream>

#include "CEngine.h"
#include "objLoader.h"
#include "stb_image.h"

#include <vector>

const GLint WIDTH = 800, HEIGHT = 800;

int main(int argc, char *argv[]) {

    CEngine c(WIDTH, HEIGHT, TEXTURED_RENDERING);

    c.getWindow().setBGColor(.02f, .21f, .04f);

    std::vector<vec3> verts;
    std::vector<vec2> texs;
    loadObj("/users/claytonknittel/downloads/cars/Low_Poly_City_Cars.obj", verts, texs);

    texture tex("/users/claytonknittel/downloads/cars/tex/Car_12.png");

    std::shared_ptr<textured_shape> tobj = c.create_shape<textured_shape>(verts, texs, tex);
    tobj->bufferData();
    tobj->setScale(.1f);

    while (!c.getWindow().shouldClose()) {
        c.draw();
    }

    return 0;
}
