
#include "renderer.h"

template<>
renderer<colored_shape>::renderer(): shader("two.vs", "two.frag") {}

template<>
renderer<textured_shape>::renderer(): shader("tex.vs", "tex.frag") {}

template<class shape>
void renderer<shape>::add(const shape &s) {
    shapes.push_back(s);
}
