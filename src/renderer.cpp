
#include "renderer.h"


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
    projLoc = shader.uniformLoc("projection");
    camLoc = shader.uniformLoc("cam");
    modelLoc = shader.uniformLoc("model");
    lightLoc = shader.uniformLoc("lightDir");
}

void renderer::render(const screen &s, scene &sc) {
    shader.use();

    s.loadProjection(projLoc);
    s.loadCameraMatrix(camLoc);

    glUniform3f(lightLoc, 1.f, 0.f, 0.f);
    
    for (const std::shared_ptr<abstract_shape> sh : sc.getShapes()) {
        sh->setModelMatrix(modelLoc);
        sh->draw();
    }
}
