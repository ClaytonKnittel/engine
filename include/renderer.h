
#include <vector>
#include "shape.h"
#include "shader.h"
#include "screen.h"
#include "camera.h"

using std::vector;


// class world {
// public:
//     world();

//     void add(colored_shape &s);
//     void add(textured_shape &s);

// private:
//     renderer<colored_shape> rc;
//     renderer<textured_shape> rt;
// };

/*
* the renderer is the object which handles the
* tracking and rendering of a specific class of
* shapes, which must inherit from abstract_shape
*/
template<class shape>
class renderer {
    // ensure that all template parameters are abstract_shapes
    static_assert(std::is_base_of<abstract_shape, shape>::value, "shape must inherit from abstract_shape");

public:
    renderer();

    virtual void render(screen &s);

    virtual void add(const shape &s);

// private:
    vector<shape> shapes;
    program shader;

    // projection and camera matrix uniform locations
    GLuint projLoc, camLoc, modelLoc;
};
