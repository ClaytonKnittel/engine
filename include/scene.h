#pragma once

#include <vector>

#include "shape.h"

using std::vector;

class scene {
public:

    scene();

    const vector<abstract_shape> &shapes() const;

private:
    vector<abstract_shape> shapes;

};
