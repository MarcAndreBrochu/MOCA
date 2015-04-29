#include <QVector>
#include <QVector3D>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include "GLBox.h"

GLBox::GLBox(QOpenGLShaderProgram *shader) :
    GLDrawable(shader),
    _vbuffer(QOpenGLBuffer::VertexBuffer),
    _cbuffer(QOpenGLBuffer::VertexBuffer) {
}
GLBox::~GLBox() {
    _vbuffer.destroy();
    _cbuffer.destroy();
    _vao.destroy();
}

void GLBox::create(float dx, float dy, float dz) {

    float dx2 = dx / 2.0f;
    float dy2 = dy / 2.0f;
    float dz2 = dz / 2.0f;

    // 6 faces * 2 triangles/face * 3 vertices/triangle = 36 vertices
    QVector<QVector3D> vertices = {
        {-dx2, -dy2,  dz2}, { dx2, -dy2,  dz2}, { dx2,  dy2,  dz2}, // Devant
        { dx2,  dy2,  dz2}, {-dx2,  dy2,  dz2}, {-dx2, -dy2,  dz2},
        { dx2, -dy2, -dz2}, {-dx2, -dy2, -dz2}, {-dx2,  dy2, -dz2}, // Derriere
        {-dx2,  dy2, -dz2}, { dx2,  dy2, -dz2}, { dx2, -dy2, -dz2},
        {-dx2, -dy2, -dz2}, {-dx2, -dy2,  dz2}, {-dx2,  dy2,  dz2}, // Gauche
        {-dx2,  dy2,  dz2}, {-dx2,  dy2, -dz2}, {-dx2, -dy2, -dz2},
        { dx2, -dy2,  dz2}, { dx2, -dy2, -dz2}, { dx2,  dy2, -dz2}, // Droite
        { dx2,  dy2, -dz2}, { dx2,  dy2,  dz2}, { dx2, -dy2,  dz2},
        {-dx2,  dy2,  dz2}, { dx2,  dy2,  dz2}, { dx2,  dy2, -dz2}, // Haut
        { dx2,  dy2, -dz2}, {-dx2,  dy2, -dz2}, {-dx2,  dy2,  dz2},
        {-dx2, -dy2, -dz2}, { dx2, -dy2, -dz2}, { dx2, -dy2,  dz2}, // Bas
        { dx2, -dy2,  dz2}, {-dx2, -dy2,  dz2}, {-dx2, -dy2, -dz2}
    };

    // On fait la meme chose avec les couleurs des faces
    // Check la composition du tableau pis essaie de dire qu'on s'en caliss pas.
    const QVector3D red(1, 0, 0); // rouge
    const QVector3D gre(0, 1, 0); // vert
    const QVector3D blu(0, 0, 1); // bleu
    QVector<QVector3D> colors = {
        red, red, red, red, red, red, // Devant
        red, red, red, red, red, red, // Derriere
        gre, gre, gre, gre, gre, gre, // Gauche
        gre, gre, gre, gre, gre, gre, // Droite
        blu, blu, blu, blu, blu, blu, // Haut
        blu, blu, blu, blu, blu, blu  // Bas
    };

    // Initialiser le buffers de vertex et de couleurs et le VAO
    _vao.create();
    _vao.bind();

    _vbuffer.create();
    _vbuffer.bind();
    _vbuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _vbuffer.allocate(vertices.constData(), vertices.size() * sizeof(QVector3D));
    _shader->enableAttributeArray("vertex");
    _shader->setAttributeBuffer("vertex", GL_FLOAT, 0, 3);

    _cbuffer.create();
    _cbuffer.bind();
    _cbuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _cbuffer.allocate(colors.constData(), colors.size() * sizeof(QVector3D));
    _shader->enableAttributeArray("color");
    _shader->setAttributeBuffer("color", GL_FLOAT, 0, 3);

    _vao.release();
}

void GLBox::draw(const QOpenGLContext *context) {

    // Bind le VAO, enable les attributs et draw. Le VAO contient deja les donnes de
    // couleur et de vertices alors ca va plus vite.
    // Comme le dit si bien notre ami Sanic teh edgydog, "GOTTA GO FAST"
    _vao.bind();
    context->functions()->glDrawArrays(GL_TRIANGLES, 0, _vbuffer.size());
    _vao.release();
}
