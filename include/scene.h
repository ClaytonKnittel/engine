#pragma once

#include <vector>

#include "shape.h"

using std::vector;

class scene {
public:

    scene();

    void addShape(std::shared_ptr<abstract_shape>);
    
    const vector<std::shared_ptr<abstract_shape>> &getShapes() const;

private:
    vector<std::shared_ptr<abstract_shape>> shapes;

};
