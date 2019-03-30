#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>

#include "texture.h"
#include "vecmath.h"

using std::vector;

// enum textureFormat {
//     // only v and vn specified in object file, color must be provided (black is default)
//     NORMALS_AND_COLOR = 1,
//     // only v and vt specified in object file, normal vectors will be computed
//     TEXCOORDS,
//     // v, vn and vt all provided in object file, nothing extra is done
//     NORMALS_AND_TEXCOORDS
// };

class abstract_shape {
public:
    // all children must call init_arrays() in their constructors
    abstract_shape(vector<vec3> vertices);
    // all children must call init_arrays() in their constructors
    abstract_shape(vector<vec3> vertices, vector<vec3> normals);

    abstract_shape();
    // abstract_shape(const char* obj_file_loc);

    ~abstract_shape();

    virtual void draw() const;

    // computes all vertex data and buffers it
    // to the GPU
    void bufferData(GLint drawMode=GL_STATIC_DRAW) const;

    void setModelMatrix(GLint modelMatrix) const;

    void setScale(float scale);
    void setPos(float x, float y, float z);
    void setPos(const vec3 &v);
    const vec3& getPos();

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

    vector<vec3> vertices, normals;

private:
    vec3 pos;
    GLuint vao, vbo;

    // how much to scale the object by when drawing
    float scale;
};

class textured_shape : public abstract_shape {
public:

    static const char* const vertex_shader_loc;
    static const char* const fragment_shader_loc;

    textured_shape(const vector<vec3> &vertices, const vector<vec2> texCoords, const texture &t);
    textured_shape(const vector<vec3> &vertices, const vector<vec3> &normals, const vector<vec2> texCoords, const texture &t);

    textured_shape(const char* obj_file_loc);
    
    virtual void draw() const;

    virtual uint vertex_size() const;

    void setColor(const vec3 &color);

protected:
    textured_shape();

    virtual void init_vertex_attributes();
    virtual void fill_data(GLfloat *data, size_t size, uint stride) const;

    vector<vec2> texCoords;
    texture tex;
};

class colored_shape : public textured_shape {
public:
    static const char* const vertex_shader_loc;
    static const char* const fragment_shader_loc;

    colored_shape(const vector<vec3> &vertices, const vec4 &color);
    colored_shape(const vector<vec3> &vertices, const vector<vec3> &normals, const vec4 &colors);

    colored_shape(const char* obj_file_loc, const vec4 &color);

    virtual uint vertex_size() const;

protected:
    virtual void init_vertex_attributes();
    // virtual void fill_data(GLfloat *data, size_t size, uint stride) const;

private:
    // vector<vec4> colors;
};
