
#include "CEngine.h"



CEngine::CEngine(GLint width, GLint height, GLint renderingStrategy): w(width, height), r(renderingStrategy), i(w) {
    w.getScreen()->getCamera().setInputs(i);
    w.setBGColor(.02f, .21f, .04f);
}

bool CEngine::shouldDraw() const {
    return !w.shouldClose();
}

void CEngine::draw() {
    w.beginDraw();
    r.render(*w.getScreen(), s);
    i.run(.01f);
    w.endDraw();
}
