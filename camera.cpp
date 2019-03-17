
#define PI 3.14159265358

#include "camera.h"

#include "input.h"
#include "math.h"
#include <cmath>
#include <functional>

using std::sin;
using std::cos;

const GLfloat camera::fvel = 1.f;
const GLfloat camera::bvel = .7f;
const GLfloat camera::pvel = .8f;
const GLfloat camera::rotvel = .05f;
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
}

void camera::forward(float dt) {
    float d = dt * fvel;
    float st = cos(theta);
    x += sin(phi) * st * d;
    z -= cos(phi) * st * d;
}

void camera::backward(float dt) {
    float d = dt * bvel;
    float st = cos(theta);
    x -= sin(phi) * st * d;
    z += cos(phi) * st * d;
}

void camera::panLeft(float dt) {
    float d = dt * pvel;
    float st = cos(theta);
    x -= cos(phi) * st * d;
    z += sin(phi) * st * d;
}

void camera::panRight(float dt) {
    float d = dt * pvel;
    float st = cos(theta);
    x += cos(phi) * st * d;
    z -= sin(phi) * st * d;
}

void camera::rotateLeft(float dt) {
    phi += dt * rotvel;
}

void camera::rotateRight(float dt) {
    phi -= dt * rotvel;
}

void camera::lookUp(float dt) {

}

void camera::lookDown(float dt) {

}

void camera::tiltLeft(float dt) {

}

void camera::tiltRight(float dt) {

}


void camera::setMatrix(float *f) const {
    // float cp = cos(phi);
    // float sp = sin(phi);
    // float ct = cos(theta);
    // float st = sin(theta);
    // float cs = cos(psi);
    // float ss = sin(psi);

    mat4 rp = mat4::rotY(phi);
    mat4 rt = mat4::rotX(theta);
    mat4 rs = mat4::rotZ(psi);
    mat4 tr = mat4::trans(-x, -y, -z);

    mat4 res = rp * rt * rs * tr;
    for (int i = 0; i < 16; i++)
        f[i] = res.m[i];

    // f[0] = cp * cs + st * sp * ss;
    // f[1] = ct * ss;
    // f[2] = cp * st * ss - cs * sp;
    // f[3] = 0;
    // f[4] = cs * st * sp - cp * ss;
    // f[5] = ct * cs;
    // f[6] = cp * cs * st + sp * ss;
    // f[7] = 0;
    // f[8] = ct * sp;
    // f[9] = -st;
    // f[10] = ct * cp;
    // f[11] = 0;
    // f[12] = -x;
    // f[13] = -y;
    // f[14] = -z;
    // f[15] = 1;
}
