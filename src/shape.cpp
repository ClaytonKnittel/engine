
#include "shape.h"
#include <cstdio>

#include "objLoader.h"

const char* const colored_shape::vertex_shader_loc = "res/two.vs";
const char* const colored_shape::fragment_shader_loc = "res/two.frag";
const char* const textured_shape::vertex_shader_loc = "res/tex.vs";
const char* const textured_shape::fragment_shader_loc = "res/tex.frag";


void compute_normals(const vector<vec3> &vertices, vector<vec3> &normals) {
    for (int i = 0; i < vertices.size(); i+=3) {
        vec3 norm = (vertices[i + 1] - vertices[i]).cross(vertices[i + 2] - vertices[i]).normalized();
        for (int j = 0; j < 3; j++)
            normals.push_back(norm);
    }
}

abstract_shape::abstract_shape(vector<vec3> vertices): vertices(vertices), pos({0.f, 0.f, 0.f}) {
    compute_normals(vertices, normals);
}

abstract_shape::abstract_shape(vector<vec3> vertices, vector<vec3> normals): vertices(vertices), normals(normals), pos({0.f, 0.f, 0.f}) {
    if (vertices.size() != normals.size()) {
        fprintf(stderr, "unequal number of vertices and normals in shape: %lu vertices and %lu normals\n", vertices.size(), normals.size());
    }
}

abstract_shape::abstract_shape(): pos({0.f, 0.f, 0.f}) {}

abstract_shape::~abstract_shape() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void abstract_shape::draw() const {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

void abstract_shape::bufferData(GLint drawMode) const {
    GLfloat *data = new GLfloat[vertex_size() * vertices.size()];
    
    const uint vsize = vertex_size();
    const size_t size = vertices.size();
    for (size_t i = 0; i < size; i++) {
        data[i * vsize] = vertices[i].x;
        data[i * vsize + 1] = vertices[i].y;
        data[i * vsize + 2] = vertices[i].z;
        data[i * vsize + 3] = normals[i].x;
        data[i * vsize + 4] = normals[i].y;
        data[i * vsize + 5] = normals[i].z;
    }
    fill_data(data + 6, size, vsize);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size * vsize, data, drawMode);
    glBindVertexArray(0);
}

void abstract_shape::setModelMatrix(GLint modelMatrix) const {
    const float m[16] = {scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, scale, 0, pos.x, pos.y, pos.z, 1.f};
    glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, m);
}

void abstract_shape::setScale(float scale) {
    this->scale = scale;
}

void abstract_shape::setPos(float x, float y, float z) {
    pos.x = x;
    pos.y = y;
    pos.z = z;
}

void abstract_shape::setPos(const vec3 &v) {
    pos = v;
}

const vec3& abstract_shape::getPos() {
    return pos;
}

void abstract_shape::init_arrays() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // always enable first two vertex attributes, which
    // will be the actual vertex and the normal
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // generate the vertex buffer object
    glGenBuffers(1, &vbo);

    // bind vbo so we can assign attributes
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // initialize vertex attributes common to all shapes
    uint vsize = vertex_size();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vsize * sizeof(GLfloat), (GLvoid*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vsize * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));

    // call child override to initialize vertex attributes
    init_vertex_attributes();

    glBindVertexArray(0);
}




textured_shape::textured_shape(const vector<vec3> &vertices, const vector<vec2> texCoords, const texture &t): abstract_shape(vertices), texCoords(texCoords), tex(t) {
    init_arrays();
    bufferData();
}

textured_shape::textured_shape(const vector<vec3> &vertices, const vector<vec3> &normals, const vector<vec2> texCoords, const texture &t): abstract_shape(vertices, normals), texCoords(texCoords), tex(t) {
    init_arrays();
    bufferData();
}

textured_shape::textured_shape(const char* obj_file_loc) : abstract_shape() {
    loadObj(obj_file_loc, &this->vertices, &this->normals, &this->texCoords);
    if (this->normals.size() == 0)
        compute_normals(this->vertices, this->normals);
    tex.load(get_texture_loc(obj_file_loc).c_str());
    init_arrays();
    bufferData();
}

textured_shape::textured_shape() : abstract_shape() {}

void textured_shape::draw() const {
    tex.use();
    this->abstract_shape::draw();
}

uint textured_shape::vertex_size() const {
    // 3 + 3 + 2
    return 8;
}

void textured_shape::setColor(const vec3 &color) {
    tex.setColor(color.x, color.y, color.z);
}

void textured_shape::init_vertex_attributes() {
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertex_size() * sizeof(GLfloat), (GLvoid*) (6 * sizeof(GLfloat)));
}

void textured_shape::fill_data(GLfloat *data, size_t size, uint stride) const {
    for (size_t i = 0; i < size; i++) {
        data[stride * i] = texCoords[i].x;
        data[stride * i + 1] = texCoords[i].y;
    }
}




colored_shape::colored_shape(const vector<vec3> &vertices, const vec4 &color): textured_shape(vertices, std::move(vector<vec2>(vertices.size())), texture(color)) {}

colored_shape::colored_shape(const vector<vec3> &vertices, const vector<vec3> &normals, const vec4 &color): textured_shape(vertices, normals, std::move(vector<vec2>(vertices.size())), texture(color)) {}

colored_shape::colored_shape(const char* obj_file_loc, const vec4 &color): textured_shape() {
    loadObj(obj_file_loc, &this->vertices, &this->normals, nullptr);

    // TODO: fix normal directions

    this->normals.clear();
    compute_normals(this->vertices, this->normals);
    
    this->texCoords = std::move(vector<vec2>(this->vertices.size()));
    for (auto it = texCoords.begin(); it != texCoords.end(); it++) {
        it->x = 0;
        it->y = 0;
    }
    tex.create(color.x, color.y, color.z, color.w);
    init_arrays();
    bufferData();
}

uint colored_shape::vertex_size() const {
    // 3 + 3 + 4
    return 10;
}

void colored_shape::init_vertex_attributes() {
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, vertex_size() * sizeof(GLfloat), (GLvoid*) (6 * sizeof(GLfloat)));
}

// void colored_shape::fill_data(GLfloat *data, size_t size, uint stride) const {
//     for (size_t i = 0; i < size; i++) {
//         data[stride * i] = colors[i].x;
//         data[stride * i + 1] = colors[i].y;
//         data[stride * i + 2] = colors[i].z;
//         data[stride * i + 3] = colors[i].w;
//     }
// }
