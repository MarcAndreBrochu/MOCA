#include <QVector>
#include <QVector3D>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include "GLSphere.h"

GLSphere::GLSphere(QOpenGLShaderProgram *shader) :
    GLDrawable(shader),
    _vbuffer(QOpenGLBuffer::VertexBuffer),
    _cbuffer(QOpenGLBuffer::VertexBuffer) {
}
GLSphere::~GLSphere() {
    _vbuffer.destroy();
    _cbuffer.destroy();
    _vao.destroy();
}

void GLSphere::create(float radius, float depth) {

    QVector<QVector3D> vertices;
    QVector<QVector3D> colors;

    const double r2 = radius / 2.f;
    const double X = 0.525731112119133606;
    const double Z = 0.850650808352039932;
    double vdata[12][3] = {
       {-X, 0, Z}, { X, 0,  Z}, {-X,  0, -Z}, { X,  0, -Z},
       { 0, Z, X}, { 0, Z, -X}, { 0, -Z,  X}, { 0, -Z, -X},
       { Z, X, 0}, {-Z, X,  0}, { Z, -X,  0}, {-Z, -X,  0}
    };

    // On represente chacune des vertices dans ce tableau d'indices
    uint tindices[20][3] = {
        {0,  4,  1}, {0, 9,  4}, {9,  5, 4}, { 4, 5, 8}, {4, 8,  1},
        {8, 10,  1}, {8, 3, 10}, {5,  3, 8}, { 5, 2, 3}, {2, 7,  3},
        {7, 10,  3}, {7, 6, 10}, {7, 11, 6}, {11, 0, 6}, {0, 1,  6},
        {6,  1, 10}, {9, 0, 11}, {9, 11, 2}, { 9, 2, 5}, {7, 2, 11}
    };

    for(int i = 19; i >= 0; i--) {

        QVector3D v1 = QVector3D(vdata[tindices[i][2]][0], vdata[tindices[i][2]][1], vdata[tindices[i][2]][2]);
        QVector3D v2 = QVector3D(vdata[tindices[i][1]][0], vdata[tindices[i][1]][1], vdata[tindices[i][1]][2]);
        QVector3D v3 = QVector3D(vdata[tindices[i][0]][0], vdata[tindices[i][0]][1], vdata[tindices[i][0]][2]);

        vertices << this->subdivideTriangle(v1, v2, v3, depth);
    }

    // Pire systeme de coloration ci-dessous:
    // on en profite aussi pour scaler les vertices par-rapport au rayon desire
    for (int i = 0; i < vertices.size(); i++) {

        vertices[i] *= r2;

        if(i % 2 == 0) colors << QVector3D(1.0, 0.0, 0.0);
        else colors << QVector3D(0.0, 0.0, 1.0);
    }

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

QVector<QVector3D> GLSphere::subdivideTriangle(QVector3D v1, QVector3D v2, QVector3D v3, int depth) {
    QVector<QVector3D> vLst;

    if (depth == 0) {
        vLst << v1 << v2 << v3;
        return vLst;
    }

    QVector3D v12 = v1 + v2;
    QVector3D v23 = v2 + v3;
    QVector3D v31 = v3 + v1;
    v12.normalize();
    v23.normalize();
    v31.normalize();

    vLst << subdivideTriangle(v1, v12, v31, depth - 1);
    vLst << subdivideTriangle(v2, v23, v12, depth - 1);
    vLst << subdivideTriangle(v3, v31, v23, depth - 1);
    vLst << subdivideTriangle(v12, v23, v31, depth - 1);

    return vLst;
}

void GLSphere::draw(const QOpenGLContext *context) {

    // Bind le VAO, enable les attributs et draw. Le VAO contient deja les donnes de
    // couleur et de vertices alors ca va plus vite.
    // Comme le dit si bien notre ami Sanic teh edgydog, "GOTTA GO FAST"
    _vao.bind();
    context->functions()->glDrawArrays(GL_TRIANGLES, 0, _vbuffer.size());
    _vao.release();
}
