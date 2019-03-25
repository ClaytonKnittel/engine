
#include "renderer.h"

// template<>
// renderer<colored_shape>::renderer(): shader("two.vs", "two.frag") {
//     camLoc = shader.uniformLoc("cam");
//     projLoc = shader.uniformLoc("projection");
//     modelLoc = shader.uniformLoc("model");
// }

// template<>
// renderer<textured_shape>::renderer(): shader("tex.vs", "tex.frag") {
//     camLoc = shader.uniformLoc("cam");
//     projLoc = shader.uniformLoc("projection");
//     modelLoc = shader.uniformLoc("model");
// }

const GLint COLORED_RENDERING = 1;
const GLint TEXTURED_RENDERING = 2;

const char* getVSLocation(const GLint rendererStrategy) {
    switch (rendererStrategy) {
    case COLORED_RENDERING:
        return "two.vs";
    case TEXTURED_RENDERING:
        return "tex.vs";
    }
    return "";
}

const char* getFSLocation(const GLint rendererStrategy) {
    switch (rendererStrategy) {
    case COLORED_RENDERING:
        return "two.frag";
    case TEXTURED_RENDERING:
        return "tex.frag";
    }
    return "";
}

renderer::renderer(const GLint rendererStrategy): shader(getVSLocation(rendererStrategy), getFSLocation(rendererStrategy)) {
}

void renderer::render(screen &s, scene &sc) {
    shader.use();

    s.loadProjection(projLoc);
    s.loadCameraMatrix(camLoc);
    
    for (const abstract_shape &sh : sc.shapes()) {
        sh.setModelMatrix(modelLoc);
        sh.draw();
    }
}
