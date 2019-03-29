#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "vecmath.h"
#include "stb_image.h"

class texture {
public:
    texture();
    texture(float r, float g, float b, float a=1.f);
    texture(const vec4 &color);
    texture(const char* loc);
    ~texture();

    void create(float r, float g, float b, float a=1.f);
    void load(const char* loc);

    void setColor(float r, float g, float b, float a=1.f);

    // binds this texture for use, can specify
    // which texture this is (are allowed up to
    // 16 textures at a time)
    void use(uint texNum = 0) const;
private:
    GLuint tex;
};
