
#include "scene.h"


scene::scene() {}

void scene::addShape(std::shared_ptr<abstract_shape> shape) {
    shapes.push_back(shape);
}

const vector<std::shared_ptr<abstract_shape>> &scene::getShapes() const {
    return shapes;
}
