#ifndef MOCA_GLDRAWABLE_H
#define MOCA_GLDRAWABLE_H

#include <QOpenGLShaderProgram>

class QOpenGLContext;

/**
 * @class GLDrawable
 * @brief Classe mere de tous les GLxxxx.
 * Definit une interface a reimplementer.
 */
class GLDrawable {

public:
    GLDrawable(QOpenGLShaderProgram *shader);
    virtual ~GLDrawable() = 0;

    // Dessine l'objet, tant que la methode est appellee dans un
    // contexte OpenGL valide
    virtual void draw(const QOpenGLContext *context) = 0;

protected:
    QOpenGLShaderProgram *_shader;
};

inline GLDrawable::GLDrawable(QOpenGLShaderProgram *shader) {
    _shader = shader;
}
inline GLDrawable::~GLDrawable() {}

#endif // MOCA_GLDRAWABLE_H
