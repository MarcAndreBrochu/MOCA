#include "GLWidget.h"
#include <QCoreApplication>
#include <QWheelEvent>
#include <QMouseEvent>

#include <MOCA/MOCA.h>
#include <MOCA/Utils.h>

GLWidget::GLWidget(const QGLFormat &format, QWidget *parent)
    : QGLWidget(format, parent),
      _vertexBuffer(QOpenGLBuffer::VertexBuffer) {

    alpha = 25;
    beta = -25;
    distance = 20;

    _timer = new QTimer(this);
    _timer->start((1.0f / 60.0f) * 1000);
    connect(_timer, SIGNAL(timeout()), this, SLOT(update()));

    // le okis
    _world = new World;
    Box *boxette = new Box(1, 1, 1);
    boxette->setPosition(0, 2, 0);
    boxette->setMass(1);

    _bodies << boxette;

    _world->addBody(boxette);
    _world->applyAcceleration(MOCA_MAKE_VEC3(0, -MOCA_METERS_TO_PIXELS(0.2), 0));
}

GLWidget::~GLWidget() {
    // [commentaire farfelu]
    if (_shaderProgram)
        delete _shaderProgram;
    _vao.destroy();
    _vertexBuffer.destroy();

    delete _world;
    for (auto it : _bodies)
        delete it;
}

void GLWidget::initializeGL() {

    // ...............................................................
    // Pre-initialisation
    // ...............................................................
    // pour QOpenGLFunctions
    if (!this->initializeOpenGLFunctions()) {
        qWarning() << "Cannot initialize OpenGL functions";
        return;
    }
    this->printOpenGLInformations();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearColor(0.0f, 0.4f, 0.4f, 1.0f);

    // Creation du VAO (on fait pas grand-chose avec pour l'instant)
    _vao.create();
    _vao.bind();

    // ...............................................................
    // Les vraies affaires
    // ...............................................................
    vertexEdgard = VerticeMaker::verticesCube(1.0);

    // Creation du shader program
    _shaderProgram = this->initializeShaderProgram(":/simple.vert", ":/simple.frag");
    _shaderProgram->bind();

    // Creation d'un buffer dans le GPU
    _vertexBuffer.create();
    _vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    if (!_vertexBuffer.bind()) {
        qWarning() << "Could not bind vertex buffer";
        return;
    }
    _vertexBuffer.allocate(vertexEdgard.vertices.size() * (3+3+3+3) * sizeof(GLfloat));

    int offset = 0;
    _vertexBuffer.write(offset, vertexEdgard.vertices.constData(), vertexEdgard.vertices.size() * 3 * sizeof(GLfloat));
    offset += vertexEdgard.vertices.size() * 3 * sizeof(GLfloat);
    _vertexBuffer.write(offset, vertexEdgard.color.constData(), vertexEdgard.vertices.size() * 3 * sizeof(GLfloat));
    offset += vertexEdgard.color.size() * 3 * sizeof(GLfloat);

    _vertexBuffer.release();
    _shaderProgram->release();
    _vao.release();
}

void GLWidget::resizeGL(int w, int h) {
    //Matrice de perspective pour garder l'aspect malgre les changements de fenetres
    pMatrix.setToIdentity();
    pMatrix.perspective(60.0, (float) w/ (float) qMax(h, 1), 0.001, 1000);

    // Mettre le viewport aux dimensions de la fenetre
    glViewport(0, 0, w, qMax(h, 1));
}

void GLWidget::update() {

    // Ne pas oublier d'appeler super
    QGLWidget::update();

    _world->updateWorld(1/60.0f);
}

void GLWidget::paintGL() {

    // On specifie la couleur qu'il faut utiliser pour clearer l'ecran
    // puis on le fait.

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _vao.bind();
/*
    // De ce que je comprends, il s'agit de binder le vertex buffer qu'on a
    // initialise plus tot puis de decrire a OpenGL se attributs. On peut
    // ensuite dessiner les vertices avec drawArrays
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer.bufferId());
    */

    //Ici on envoie ce qu'on veut aux programmes de shaders
    QMatrix4x4 mMatrix;
    QMatrix4x4 vMatrix;
    mMatrix.setToIdentity();
    vMatrix.setToIdentity();

    AbstractBody *lelcorptere = _bodies[0];
    double tx = lelcorptere->getPosition()[0];
    double ty = lelcorptere->getPosition()[1];
    double tz = lelcorptere->getPosition()[2];
    mMatrix.translate(tx, ty, tz);

    Box *lelbox = (Box *)lelcorptere;
    double rx = lelbox->getAngularPosition()[0];
    double ry = lelbox->getAngularPosition()[1];
    double rz = lelbox->getAngularPosition()[2];
    mMatrix.rotate(rx, 1, 0, 0);
    mMatrix.rotate(ry, 0, 1, 0);
    mMatrix.rotate(rz, 0, 0, 1);

    //Initialisation de la camera...
    QMatrix4x4 cameraTransformation;
    cameraTransformation.rotate(alpha, 0, 1, 0);
    cameraTransformation.rotate(beta, 1, 0, 0);

    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, distance);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);

    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    _shaderProgram->bind();
    _shaderProgram->setUniformValue("mvpMatrix", pMatrix * vMatrix * mMatrix);

    //Bindons le buffer d'Edgard
    int offset = 0;
    _vertexBuffer.bind();

    _shaderProgram->setAttributeBuffer("vertex", GL_FLOAT, offset, 3, 0);
    offset += vertexEdgard.vertices.size() * 3 * sizeof(GL_FLOAT);
    _shaderProgram->enableAttributeArray("vertex");

    _shaderProgram->setAttributeBuffer("color", GL_FLOAT, offset, 3, 0);
    offset += vertexEdgard.color.size() * 3 * sizeof(GL_FLOAT);
    _shaderProgram->enableAttributeArray("color");

/**
    _shaderProgram->setAttributeArray("vertex", verticesDEdgard.constData());
    _shaderProgram->enableAttributeArray("vertex");

    _shaderProgram->setAttributeArray("color", couleurDEdgard.constData());
    _shaderProgram->enableAttributeArray("color");

**/

//    qDebug() << "vertex" << _shaderProgram->attributeLocation("vertex");
//    qDebug() << "color" << _shaderProgram->attributeLocation("color");
//    qDebug() << "mvpMatrix" << _shaderProgram->uniformLocation("mvpMatrix");

    //On dessine Edgard le triangle
    glDrawArrays(GL_TRIANGLES, 0, vertexEdgard.vertices.size());

    _shaderProgram->setAttributeBuffer("vertex", GL_FLOAT, offset, 3, 0);
    offset += vertexRoberto.vertices.size() * 3 * sizeof(GL_FLOAT);
    _shaderProgram->enableAttributeArray("vertex");

    _shaderProgram->setAttributeBuffer("color", GL_FLOAT, offset, 3, 0);
    offset += vertexRoberto.color.size() * 3 * sizeof(GL_FLOAT);
    _shaderProgram->enableAttributeArray("color");


    //On dessine Roberto la sphere
    glDrawArrays(GL_TRIANGLES, 0, vertexRoberto.vertices.size());

    // Un simple cleanup
    _shaderProgram->disableAttributeArray("color");
    _shaderProgram->disableAttributeArray("vertex");
    _shaderProgram->release();
    _vao.release();
}

QOpenGLShaderProgram *GLWidget::initializeShaderProgram(const QString &vshPath, const QString &fshPath) {

    QOpenGLShaderProgram *prog = new QOpenGLShaderProgram;

    // Tel un souajet, j'utilise les helpers de Qt afin de loader les shaders depuis leur
    // fichiers source respectifs
    bool result = prog->addShaderFromSourceFile(QOpenGLShader::Vertex, vshPath);
    if (!result)
        qWarning() << prog->log();

    result = prog->addShaderFromSourceFile(QOpenGLShader::Fragment, fshPath);
    if (!result)
        qWarning() << prog->log();

    // Maintenant, il faut linker ensemble les deux shaders
    result = prog->link();
    if (!result) {
        qWarning() << "Could not link shader program:" << prog->log();
        delete prog;
    }

    return prog;
}

void GLWidget::printOpenGLInformations() {
    qDebug() << "Widget OpenGl: " << this->format().majorVersion() << "." << this->format().minorVersion();
    qDebug() << "Context valid: " << this->context()->isValid();
    qDebug() << "Really used OpenGl: " << this->context()->format().majorVersion() << "." << this->context()->format().minorVersion();
    qDebug() << "OpenGl information: VENDOR:       " << (const char*)glGetString(GL_VENDOR);
    qDebug() << "                    RENDERDER:    " << (const char*)glGetString(GL_RENDERER);
    qDebug() << "                    VERSION:      " << (const char*)glGetString(GL_VERSION);
    qDebug() << "                    GLSL VERSION: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
}
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastMousePosition = event->pos();

    event->accept();
}


void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int deltaX = event->x() - lastMousePosition.x();
    int deltaY = event->y() - lastMousePosition.y();

    if (event->buttons() & Qt::LeftButton) {
        alpha -= deltaX;
        while (alpha < 0) {
            alpha += 360;
        }
        while (alpha >= 360) {
            alpha -= 360;
        }

        beta -= deltaY;
        if (beta < -90) {
            beta = -90;
        }
        if (beta > 90) {
            beta = 90;
        }

        updateGL();
    }

    lastMousePosition = event->pos();

    event->accept();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    int delta = event->delta();

    if (event->orientation() == Qt::Vertical) {
        if (delta < 0) {
            distance *= 1.1;
        } else if (delta > 0) {
            distance *= 0.9;
        }

        updateGL();
    }

    event->accept();
}

void GLWidget::keyPressEvent(QKeyEvent *event) {

    if (event->key() == Qt::Key_Space) {

        AbstractBody *bodey = _bodies[0];
        bodey->applyImpulse(MOCA_MAKE_VEC3(0, 0.7, 0));

        Box *lel = (Box *)bodey;
        lel->applyAngularImpulse(MOCA_MAKE_VEC3(0, 0.6, 1));
    }
}
