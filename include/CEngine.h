#pragma once

#ifndef CENGINE_H
#define CENGINE_H
#endif

#define GLEW_STATIC
#include <GL/glew.h>

#include "window.h"
#include "scene.h"
#include "renderer.h"
#include "input.h"


class CEngine {
public:

    CEngine(GLint width, GLint height);

private:
    window w;
    scene s;
    renderer r;
    input_handler i;
};

