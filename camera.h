
#define GLEW_STATIC
#include <GL/glew.h>

class InputHandler;

struct camera {
    GLfloat x, y, z;

    // positive phi is a rotation about y-axis
    // positive theta is tilting your head upwards
    // positive psi is a rotation about z relative to the camera
    GLfloat phi, theta, psi; // euler angles

    camera();

    void setInputs(InputHandler &i);

    void forward(float dt);
    void backward(float dt);
    void down(float dt);
    void up(float dt);
    void panLeft(float dt);
    void panRight(float dt);
    void rotateLeft(float dt);
    void rotateRight(float dt);
    void lookUp(float dt);
    void lookDown(float dt);
    void tiltLeft(float dt);
    void tiltRight(float dt);

    // expects a 16-long array of floats preallocated
    void setMatrix(float *f) const;

    static const GLfloat fvel;
    static const GLfloat bvel;
    static const GLfloat pvel;
    static const GLfloat vvel;
    static const GLfloat rotvel;
    static const GLfloat thetavel;
    static const GLfloat tiltvel;
};
