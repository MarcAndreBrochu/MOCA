#ifndef MOCA_GLBOX_H
#define MOCA_GLBOX_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "GLDrawable.h"

class QOpenGLShaderProgram;
class QOpenGLContext;

/**
 * @class GLBox
 * @brief Permet l'affichage d'une boite.
 * Contient toutes les donnees (VAO, VBOs, etc.) necessaires pour que OpenGL
 * puisse dessiner a l'ecran une boite.
 */
class GLBox : public GLDrawable {

public:
    GLBox(QOpenGLShaderProgram *shader);
    virtual ~GLBox();

    // Va remplir les buffers, etc.
    void create(float dx, float dy, float dz);

    virtual void draw(const QOpenGLContext *context);

private:
    QOpenGLVertexArrayObject _vao;
    QOpenGLBuffer _vbuffer; // vertices
    QOpenGLBuffer _cbuffer; // couleurs
};

#endif // MOCA_GLBOX_H
