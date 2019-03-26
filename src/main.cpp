
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

    // w.setBGColor(.02f, .21f, .04f);

    CEngine c(WIDTH, HEIGHT, TEXTURED_RENDERING);

    // program shader("two.vs", "two.frag");
    // program shader("tex.vs", "tex.frag");

    std::vector<vec3> verts;
    std::vector<vec2> texs;
    loadObj("/users/claytonknittel/downloads/cars/Low_Poly_City_Cars.obj", verts, texs);

    texture tex("/users/claytonknittel/downloads/cars/tex/Car_12.png");
    // textured_shape tobj(verts, texs, tex);
    // tobj.bufferData();
    // tobj.setScale(.1f);

    // r.add(tobj);

    std::shared_ptr<textured_shape> tobj = c.create_shape<textured_shape>(verts, texs, tex);
    tobj->bufferData();
    tobj->setScale(.1f);

    while (c.shouldDraw()) {
        c.draw();
    }

    return 0;
}
