#pragma once

#include <cmath>

#define GLEW_STATIC
#include <GL/glew.h>

typedef unsigned int uint;

void loadPerspectiveProjection(GLint matrix, float fovy, float aspect, float znear, float zfar);

struct vec3;

struct vec4 {
    GLfloat x, y, z, w;

    vec4(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    vec4(const vec3 &v);

    GLfloat dot(const vec4 &v) const;
    GLfloat mag() const;
    vec4 normalized() const;
};

struct vec3 {
    GLfloat x, y, z;

    vec3 operator+(const vec3 &v) const;
    vec3 operator-(const vec3 &v) const;
    vec3 operator*(GLfloat f) const;
    vec3 operator/(GLfloat f) const;
    
    GLfloat dot(const vec3 &v) const;
    vec3 cross(const vec3 &v) const;
    GLfloat mag() const;
    vec3 normalized() const;
};

struct vec2 {
    GLfloat x, y;

    GLfloat dot(const vec2 &v) const;
    GLfloat mag() const;
    vec2 normalized() const;
};

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
