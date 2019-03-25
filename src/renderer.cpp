
#include "renderer.h"

template<>
renderer<colored_shape>::renderer(): shader("two.vs", "two.frag") {
    camLoc = shader.uniformLoc("cam");
    projLoc = shader.uniformLoc("projection");
    modelLoc = shader.uniformLoc("model");
}

template<>
renderer<textured_shape>::renderer(): shader("tex.vs", "tex.frag") {
    camLoc = shader.uniformLoc("cam");
    projLoc = shader.uniformLoc("projection");
    modelLoc = shader.uniformLoc("model");
}

template<class shape>
void renderer<shape>::render(screen &s) {
    shader.use();

    s.loadProjection(projLoc);
    s.loadCameraMatrix(camLoc);
    
    for (shape &sh : shapes) {
        sh.setModelMatrix(modelLoc);
        sh.draw();
    }
}

template<class shape>
void renderer<shape>::add(const shape &s) {
    shapes.push_back(s);
}
