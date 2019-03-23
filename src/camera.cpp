
#define PI 3.14159265358

#include "camera.h"

#include "input.h"
#include "vecmath.h"
#include <cmath>
#include <functional>

using std::sin;
using std::cos;

const GLfloat camera::fvel = 1.f;
const GLfloat camera::bvel = .7f;
const GLfloat camera::pvel = .8f;
const GLfloat camera::vvel = .7f;
const GLfloat camera::rotvel = .2f;
const GLfloat camera::thetavel = .05f;
const GLfloat camera::tiltvel = .05f;


camera::camera(): x(0), y(0), z(0), phi(0), theta(0), psi(0) {}

void camera::setInputs(InputHandler &i) {
    using namespace std::placeholders;
    i.add(GLFW_KEY_W, std::bind(&camera::forward, this, _1));
    i.add(GLFW_KEY_S, std::bind(&camera::backward, this, _1));
    i.add(GLFW_KEY_D, std::bind(&camera::rotateRight, this, _1));
    i.add(GLFW_KEY_A, std::bind(&camera::rotateLeft, this, _1));

    i.add(GLFW_KEY_Q, std::bind(&camera::panLeft, this, _1));
    i.add(GLFW_KEY_E, std::bind(&camera::panRight, this, _1));
    i.add(GLFW_KEY_X, std::bind(&camera::up, this, _1));
    i.add(GLFW_KEY_Z, std::bind(&camera::down, this, _1));

    i.add(GLFW_KEY_F, std::bind(&camera::lookDown, this, _1));
    i.add(GLFW_KEY_R, std::bind(&camera::lookUp, this, _1));
}

void camera::forward(float dt) {
    float d = dt * fvel;
    x -= sin(phi) * d;
    z -= cos(phi) * d;
}

void camera::backward(float dt) {
    float d = dt * bvel;
    x += sin(phi) * d;
    z += cos(phi) * d;
}

void camera::down(float dt) {
    y -= dt * vvel;
}

void camera::up(float dt) {
    y += dt * vvel;
}

void camera::panLeft(float dt) {
    float d = dt * pvel;
    x -= cos(phi) * d;
    z += sin(phi) * d;
}

void camera::panRight(float dt) {
    float d = dt * pvel;
    x += cos(phi) * d;
    z -= sin(phi) * d;
}

void camera::rotateLeft(float dt) {
    phi += dt * rotvel;
}

void camera::rotateRight(float dt) {
    phi -= dt * rotvel;
}

void camera::lookUp(float dt) {
    theta += dt * thetavel;
}

void camera::lookDown(float dt) {
    theta -= dt * thetavel;
}

void camera::tiltLeft(float dt) {
    psi += dt * tiltvel;
}

void camera::tiltRight(float dt) {
    psi -= dt * tiltvel;
}


void camera::setMatrix(float *f) const {

    mat4 rp = mat4::rotY(-phi);
    mat4 rt = mat4::rotX(-theta);
    mat4 rs = mat4::rotZ(-psi);
    mat4 tr = mat4::trans(-x, -y, -z);

    mat4 res = rs * rt * rp * tr;
    memcpy(f, res.m, 16 * sizeof(float));
}
