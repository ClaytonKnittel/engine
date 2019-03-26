
#include "screen.h"
#include "vecmath.h"


screen::screen(GLfloat fov, GLfloat aspectRatio, GLfloat viewDistance): fov(fov), aspectRatio(aspectRatio), viewDistance(viewDistance) {
    // cam.setInputs(*parent->input);
}

void screen::loadProjection(GLuint projectionMatrix) const {
    loadPerspectiveProjection(projectionMatrix, fov, aspectRatio, 1.f, viewDistance);
}

void screen::loadCameraMatrix(GLuint camMatrix) const {
    GLfloat c[16];
    cam.setMatrix(c);
    glUniformMatrix4fv(camMatrix, 1, GL_FALSE, c);
}

camera &screen::getCamera() {
    return cam;
}