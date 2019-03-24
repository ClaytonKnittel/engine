
#include "texture.h"

texture::texture(const char* loc) {
    int twidth, theight, nrChannels;
    unsigned char *data = stbi_load(loc, &twidth, &theight, &nrChannels, 0);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        fprintf(stderr, "Failed to load texture\n");
    }

    stbi_image_free(data);
}

texture::~texture() {
    glDeleteTextures(1, &tex);
}

void texture::use(uint texNum) const {
    glActiveTexture(GL_TEXTURE0 + texNum);
    glBindTexture(GL_TEXTURE_2D, tex);
}
