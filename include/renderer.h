
#include <vector>
#include "shape.h"
#include "shader.h"
#include "screen.h"
#include "scene.h"

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


const extern GLint COLORED_RENDERING;
const extern GLint TEXTURED_RENDERING;

/*
* the renderer is the object which handles the
* rendering of the entities in the scene
*/
class renderer {

public:
    renderer(const GLint rendererStrategy);

    virtual void render(screen &, scene &);

private:
    program shader;

    // projection and camera matrix uniform locations
    GLuint projLoc, camLoc, modelLoc;
};