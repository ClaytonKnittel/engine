
#include "math.h"
#include <cmath>

void loadPerspectiveProjection(GLint matrix, float fovy, float aspect, float znear, float zfar) {
    const float f = 1 / std::tan(fovy / 2);
    const float proj[16] = {f / aspect, 0, 0, 0, 0, f, 0, 0, 0, 0, (zfar + znear) / (znear - zfar), 2 * zfar * znear / (znear - zfar), 0, 0, -1, 0};
    glUniformMatrix4fv(matrix, 1, GL_FALSE, proj);
}

