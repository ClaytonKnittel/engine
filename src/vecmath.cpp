
#include "vecmath.h"
#include <cmath>

#include <cstring>

#define PI 3.14159265358

#define GLEW_STATIC
#include <GL/glew.h>

using std::cos;
using std::sin;

void loadPerspectiveProjection(GLint matrix, float fovy, float aspect, float znear, float zfar) {
    const float f = 1 / std::tan(PI * fovy / 360.f);
    const float proj[16] = {f / aspect, 0, 0, 0, 0, f, 0, 0, 0, 0, (zfar + znear) / (znear - zfar), -1, 0, 0, 2 * zfar * znear / (znear - zfar), 0};
    glUniformMatrix4fv(matrix, 1, GL_FALSE, proj);
}

    
mat4::mat4() {}

mat4::mat4(const mat4 &mat) {
    for (int i = 0; i < 16; i++)
        m[i] = mat.m[i];
}

mat4& mat4::operator=(const mat4 &mat) {
    memcpy(m, mat.m, 16 * sizeof(float));
    return *this;
}

mat4 mat4::operator+(const mat4 &mat) const {
    mat4 res;
    for (int i = 0; i < 16; i++)
        res.m[i] = m[i] + mat.m[i];
    return res;
}

mat4 mat4::operator*(const mat4 &mat) const {
    mat4 res;
    res.zero();
    for (int i = 0; i < 16; i += 4)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                res.m[j + i] += m[j + 4 * k] * mat.m[k + i];
    return res;
}

mat4 mat4::eye() {
    mat4 mat;
    mat.eyefy();
    return mat;
}

mat4 mat4::rotX(float angle) {
    float ca = cos(angle);
    float sa = sin(angle);
    mat4 m;
    m.eyefy();
    m.m[5] = ca;
    m.m[6] = sa;
    m.m[9] = -sa;
    m.m[10] = ca;
    return m;
}

mat4 mat4::rotY(float angle) {
    float ca = cos(angle);
    float sa = sin(angle);
    mat4 m;
    m.eyefy();
    m.m[0] = ca;
    m.m[2] = -sa;
    m.m[8] = sa;
    m.m[10] = ca;
    return m;
}

mat4 mat4::rotZ(float angle) {
    float ca = cos(angle);
    float sa = sin(angle);
    mat4 m;
    m.eyefy();
    m.m[0] = ca;
    m.m[1] = sa;
    m.m[4] = -sa;
    m.m[5] = ca;
    return m;
}

mat4 mat4::trans(float dx, float dy, float dz) {
    mat4 m;
    m.eyefy();
    m.m[12] = dx;
    m.m[13] = dy;
    m.m[14] = dz;
    return m;
}

void mat4::zero() {
    for (int i = 0; i < 16; i++)
        m[i] = 0;
}

void mat4::eyefy() {
    zero();
    m[0] = 1;
    m[5] = 1;
    m[10] = 1;
    m[15] = 1;
}

