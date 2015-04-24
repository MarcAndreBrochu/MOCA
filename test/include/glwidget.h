#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions_3_3_Core>
#include <QVector>
#include <QOpenGLWidget>
#include <QTimer>

#include <MOCA/MOCA.h>
#include <MOCA/Utils.h>

#include "verticemaker.h"

// Code de base d'un Widget affichant du OpenGL 3.x+.
class GLWidget : public QGLWidget, protected QOpenGLFunctions_3_3_Core {

    Q_OBJECT

public:
    GLWidget(const QGLFormat &format, QWidget *parent = 0);
    ~GLWidget();

public slots:
    virtual void update();

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    QOpenGLShaderProgram *initializeShaderProgram(const QString &vshPath, const QString &fshPath);
    void printOpenGLInformations();

    QOpenGLShaderProgram *_shaderProgram;
    QOpenGLBuffer _vertexBuffer;
    QOpenGLVertexArrayObject _vao;

    vertex vertexEdgard;
    QVector<QVector3D> verticesDEdgard;
    QVector<QVector3D> couleurDEdgard;

    QOpenGLBuffer _vertexBufferRoberto;

    vertex vertexRoberto;
    QVector<QVector3D> verticesRoberto;

    QMatrix4x4 pMatrix;

    double alpha;
    double beta;
    double distance;
    QPoint lastMousePosition;

    QTimer *_timer;

    World *_world;
    QVector<AbstractBody *> _bodies;
};

#endif // GLWIDGET_H
