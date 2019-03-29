#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "window.h"
#include "scene.h"
#include "renderer.h"
#include "input.h"


// to be defined in renderer
const extern GLint COLORED_RENDERING;
const extern GLint TEXTURED_RENDERING;

class CEngine {
public:

    CEngine(GLint width, GLint height, GLint renderingStrategy);

    template<class shape, class ... Types>
    std::shared_ptr<shape> create_shape(Types&&... params) {
        std::shared_ptr<shape> ptr = std::make_shared<shape>(params...);
        s.addShape(std::static_pointer_cast<abstract_shape>(ptr));
        return ptr;
    }

    window &getWindow();

    void draw();

private:
    window w;
    scene s;
    renderer r;
    input_handler i;
};

