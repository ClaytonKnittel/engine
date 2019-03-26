
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <iostream>

#include "CEngine.h"
#include "objLoader.h"
#include "stb_image.h"

#include <vector>
#include <string>

const GLint WIDTH = 800, HEIGHT = 800;

int main(int argc, char *argv[]) {

    CEngine c(WIDTH, HEIGHT, TEXTURED_RENDERING);

    c.getWindow().setBGColor(.02f, .21f, .04f);

    std::shared_ptr<textured_shape> tobj = c.create_shape<textured_shape>("/users/claytonknittel/downloads/cars/Low_Poly_City_Cars.obj");
    tobj->setScale(.1f);

    while (!c.getWindow().shouldClose()) {
        c.draw();
    }

    return 0;
}
