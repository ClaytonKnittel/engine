
#include <vector>
#include "shape.h"
#include "shader.h"

using std::vector;

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

    virtual void add(const shape &s);

private:
    vector<shape> shapes;
    program shader;
};
