
#include <cmath>

#define GLEW_STATIC
#include <GL/glew.h>

void loadPerspectiveProjection(GLint matrix, float fovy, float aspect, float znear, float zfar);


struct mat4 {
    float m[16];
    
    mat4();
    mat4(const mat4 &mat);
    mat4& operator=(const mat4 &mat);

    mat4 operator+(const mat4 &mat) const;
    mat4 operator*(const mat4 &mat) const;

    static mat4 eye();
    static mat4 rotX(float angle);
    static mat4 rotY(float angle);
    static mat4 rotZ(float angle);
    static mat4 trans(float dx, float dy, float dz);

    void zero();
    void eyefy();

};
