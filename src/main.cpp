
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <iostream>

#include "CEngine.h"
#include "objLoader.h"
#include "vecmath.h"

#include <vector>
#include <string>
#include <sstream>

const GLint WIDTH = 800, HEIGHT = 800;

int main(int argc, char *argv[]) {

    CEngine c(WIDTH, HEIGHT, TEXTURED_RENDERING);

    c.getWindow().setBGColor(.02f, .21f, .04f);

    // std::vector<std::shared_ptr<textured_shape>> tobjs;
    // for (int i = 2; i < 12; i++) {
    //     string num;
    //     std::ostringstream is;
    //     is << i;
    //     num += is.str();
    //     string loc = "/users/claytonknittel/downloads/cars/Low_Poly_City_Cars_";
    //     loc += num;
    //     loc += "_copy.obj";
    //     tobjs.push_back(c.create_shape<textured_shape>(loc.c_str()));
    //     tobjs[tobjs.size() - 1]->setScale(.05f);
    // }

    vec4 col = {.2f, .5f, .47f, 1.f};

    std::shared_ptr<colored_shape> tobj = c.create_shape<colored_shape>("/users/claytonknittel/downloads/test.obj", col);
    tobj->setPos(0, 0, -30);
    tobj->setScale(4.f);
    long t = 0;

    while (!c.getWindow().shouldClose()) {
        t++;
        // tobjs[11]->setPos(0, 0, -static_cast<float>(t) / 100.f);
        c.draw();
    }

    return 0;
}
