#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>

#include "texture.h"
#include "vecmath.h"

using std::vector;

class abstract_shape {
public:
    // all children must call init_arrays() in their constructors
    abstract_shape(vector<vec3> vertices);
    // all children must call init_arrays() in their constructors
    abstract_shape(vector<vec3> vertices, vector<vec3> normals);
    ~abstract_shape();

    virtual void draw() const;

    // computes all vertex data and buffers it
    // to the GPU
    void bufferData() const;

    void setModelMatrix(GLint modelMatrix) const;

    void setScale(float scale);

    // specifies number of floats per vertex
    // (i.e. 6 if you only have vertices and normals,
    // 3 per vertex and 3 per normal)
    virtual uint vertex_size() const = 0;

protected:
    virtual void init_vertex_attributes() = 0;

    // fill in the remainder of the vertex array with
    // specific attributes of the child class
    virtual void fill_data(GLfloat *data, size_t size, uint stride) const = 0;

    // do initialization of the vao
    void init_arrays();

private:
    vector<vec3> vertices, normals;
    GLuint vao, vbo;

    // how much to scale the object by when drawing
    float scale;
};

class colored_shape : public abstract_shape {
public:
    static const char* const vertex_shader_loc;
    static const char* const fragment_shader_loc;

    colored_shape(vector<vec3> &vertices, vector<vec4> colors);
    colored_shape(vector<vec3> &vertices, vector<vec3> &normals, vector<vec4> colors);

    virtual uint vertex_size() const;

protected:
    virtual void init_vertex_attributes();
    virtual void fill_data(GLfloat *data, size_t size, uint stride) const;

private:
    vector<vec4> colors;
};

class textured_shape : public abstract_shape {
public:
    static const char* const vertex_shader_loc;
    static const char* const fragment_shader_loc;

    textured_shape(vector<vec3> &vertices, vector<vec2> texCoords, texture &t);
    textured_shape(vector<vec3> &vertices, vector<vec3> &normals, vector<vec2> texCoords, texture &t);
    
    virtual void draw() const;

    virtual uint vertex_size() const;

protected:
    virtual void init_vertex_attributes();
    virtual void fill_data(GLfloat *data, size_t size, uint stride) const;

private:
    vector<vec2> texCoords;
    texture tex;
};
