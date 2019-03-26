
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <iostream>

#include "CEngine.h"
#include "objLoader.h"
#include "stb_image.h"

#include <vector>
#include <string>
#include <sstream>

const GLint WIDTH = 800, HEIGHT = 800;

int main(int argc, char *argv[]) {

    CEngine c(WIDTH, HEIGHT, TEXTURED_RENDERING);

    c.getWindow().setBGColor(.02f, .21f, .04f);

    std::vector<std::shared_ptr<textured_shape>> tobjs;
    for (int i = 1; i <= 12; i++) {
        string num;
        std::ostringstream is;
        is << i;
        num += is.str();
        string loc = "/users/claytonknittel/downloads/cars/Low_Poly_City_Cars_";
        loc += num;
        loc += ".obj";
        tobjs.push_back(c.create_shape<textured_shape>(loc.c_str()));
        tobjs[tobjs.size() - 1]->setScale(.5f);
    }

    while (!c.getWindow().shouldClose()) {
        c.draw();
    }

    return 0;
}
