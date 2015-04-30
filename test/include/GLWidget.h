#ifndef MOCA_GLWIDGET_H
#define MOCA_GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions_3_3_Core>
#include <QVector>
#include <QOpenGLWidget>
#include <QTimer>

#include <MOCA/MOCA.h>
#include <MOCA/Utils.h>

class GLDrawable;

// Code de base d'un Widget affichant du OpenGL 3.x+.
class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {

    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    // Fonctions qui s'occupent d'ajouter des formes au monde a simuler et qui s'occupent
    // d'ajouter ces formes a l'affichage
    Box *createBox(double dx, double dy, double dz);
    Ball *createSphere(double radius);

public slots:
    virtual void update();

protected:
    // Appele avant que le widget ne s'affiche
    virtual void initializeGL();
    // Appele lorsque la fenetre est resize
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

    // Methodes qui recoivent l'input de l'utilisateur
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

private:
    QOpenGLShaderProgram *initializeShaderProgram(const QString &vshPath, const QString &fshPath);
    void printOpenGLInformations();

    // Fonction qui calcule une matrice de projection a la glLookAt
    QMatrix4x4 cameraLookAt(double a, double b, double d);

    QOpenGLShaderProgram *_shaderProgram;

    typedef QPair<AbstractBody *, GLDrawable *> PhysicalDrawPair;
    QVector<PhysicalDrawPair> _objectsInWorld;

    QMatrix4x4 _pMatrix;
    double _alpha;
    double _beta;
    double _distance;
    QPoint _lastMousePosition;

    QTimer *_timer;

    World *_world;

    bool _initialized;
};

#endif // MOCA_GLWIDGET_H
