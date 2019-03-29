
#include "CEngine.h"



CEngine::CEngine(GLint width, GLint height, GLint renderingStrategy): w(width, height), r(renderingStrategy), i(w) {
    w.getScreen()->getCamera().setInputs(i);
}

window &CEngine::getWindow() {
    return w;
}

const GLint WIDTH = 800, HEIGHT = 800;

void CEngine::draw() {
    w.beginDraw();
    r.render(*w.getScreen(), s);
    i.run(.01f);
    w.endDraw();
}
