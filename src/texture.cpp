
#include "texture.h"

texture::texture(): tex(0) {}

texture::texture(float r, float g, float b, float a) {
    create(r, g, b, a);
}

texture::texture(const vec4 &color): texture(color.x, color.y, color.z, color.w) {}

texture::texture(const char* loc) {
    load(loc);
}

void texture::create(float r, float g, float b, float a) {
    if (tex != 0) {
        fprintf(stderr, "Texture initialized more than once\n");
        return;
    }
    setColor(r, g, b, a);
}

void texture::load(const char* loc) {
    if (tex != 0) {
        fprintf(stderr, "Texture initialized more than once\n");
        return;
    }
    int twidth, theight, nrChannels;
    unsigned char *data = stbi_load(loc, &twidth, &theight, &nrChannels, 0);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    // printf("width: %d\nheight: %d\nnChannels: %d\n", twidth, theight, nrChannels);
    // for (uint i = 0; data[i] != '\0'; ++i) {
    //     printf("%x ", data[i]);
    // }
    // printf("\n");

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        fprintf(stderr, "Failed to load texture at \"%s\"\n", loc);
    }

    stbi_image_free(data);
}

texture::~texture() {
    glDeleteTextures(1, &tex);
}

void texture::setColor(float r, float g, float b, float a) {
    if (r < 0 || r > 1 || g < 0 || g > 1 || b < 0 || b > 1 || a < 0 || a > 1) {
        fprintf(stderr, "Invalid color in texture initialization (%f, %f, %f, %f)\n", r, g, b, a);
        return;
    }
    if (tex != 0)
        glDeleteTextures(1, &tex);
    glGenTextures(1, &tex);
    unsigned char data[4] = {static_cast<unsigned char>(255 * r),
                             static_cast<unsigned char>(255 * g),
                             static_cast<unsigned char>(255 * b),
                             static_cast<unsigned char>(255 * a)};
    printf("%u, %u, %u, %u\n", data[0], data[1], data[2], data[3]);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void texture::use(uint texNum) const {
    if (tex == 0) {
        // fprintf(stderr, "Warning: texture uninitialized\n");
        return;
    }
    glActiveTexture(GL_TEXTURE0 + texNum);
    glBindTexture(GL_TEXTURE_2D, tex);
}
