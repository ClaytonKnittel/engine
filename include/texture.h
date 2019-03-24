#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "vecmath.h"
#include "stb_image.h"

class texture {
public:
    texture(const char* loc);
    ~texture();

    // binds this texture for use, can specify
    // which texture this is (are allowed up to
    // 16 textures at a time)
    void use(uint texNum = 0) const;
private:
    GLuint tex;
};
