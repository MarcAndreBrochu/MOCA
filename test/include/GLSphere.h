#ifndef MOCA_GLSPHERE_H
#define MOCA_GLSPHERE_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "GLDrawable.h"

class QOpenGLShaderProgram;
class QOpenGLContext;

/**
 * @class GLSphere
 * @brief Permet l'affichage d'une sphere.
 * Contient toutes les donnees (VAO, VBOs, etc.) necessaires pour que OpenGL
 * puisse dessiner a l'ecran une sphere.
 */
class GLSphere : public GLDrawable {

public:
    GLSphere(QOpenGLShaderProgram *shader);
    virtual ~GLSphere();

    // Va remplir les buffers, etc.
    void create(float radius, float depth);

    virtual void draw(const QOpenGLContext *context);

private:
    QVector<QVector3D> subdivideTriangle(QVector3D v1, QVector3D v2, QVector3D v3, int depth);

    QOpenGLVertexArrayObject _vao;
    QOpenGLBuffer _vbuffer; // vertices
    QOpenGLBuffer _cbuffer; // couleurs
};

#endif // MOCA_GLSPHERE_H
