#include "GLWidget.h"
#include <QCoreApplication>
#include <QWheelEvent>
#include <QMouseEvent>

#include <MOCA/MOCA.h>
#include <MOCA/Utils.h>

#include "GLDrawable.h"
#include "GLBox.h"
#include "GLSphere.h"

GLWidget::GLWidget(QWidget *parent) :
    QOpenGLWidget(parent) {

    _alpha = 25;
    _beta = -25;
    _distance = 2.5;

    _timer = new QTimer(this);
    _timer->start((1.0f / 60.0f) * 1000);
    connect(_timer, SIGNAL(timeout()), this, SLOT(update()));

    _world = new World;
}

GLWidget::~GLWidget() {

    if (_shaderProgram)
        delete _shaderProgram;

    for (auto it : _objectsInWorld)
        delete it.second;

    delete _world;
}

Box *GLWidget::createBox(double dx, double dy, double dz) {

    Box *pbox = new Box(dx, dy, dz);
    GLBox *globj = new GLBox(_shaderProgram);
    globj->create(dx, dy, dz);

    _objectsInWorld.push_back({pbox, globj});

    return pbox;
}

Ball *GLWidget::createSphere(double radius) {

    Ball *pball = new Ball(radius);
    GLSphere *globj = new GLSphere(_shaderProgram);
    globj->create(radius, 4);

    _objectsInWorld.push_back({pball, globj});

    return pball;
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

    // ...............................................................
    // Les vraies affaires
    // ...............................................................
    // Creation du shader program (premiere chose a faire!)
    _shaderProgram = this->initializeShaderProgram(":/simple.vert", ":/simple.frag");

    Box *lel1 = this->createBox(0.1, 1, 1);
    lel1->applyAngularImpulse(MOCA_MAKE_VEC3(1, 0, 0));

    Ball *lel = this->createSphere(1);
    lel->applyAngularImpulse(MOCA_MAKE_VEC3(0, 1, 0));

    _world->addBody(lel1);
    _world->addBody(lel);
}

void GLWidget::resizeGL(int w, int h) {

    // Matrice de perspective pour garder l'aspect malgre les changements de fenetres
    _pMatrix.setToIdentity();
    _pMatrix.perspective(60.0, (float)w / (float)qMax(h, 1), 0.001, 1000);

    // Mettre le viewport aux dimensions de la fenetre
    glViewport(0, 0, w, qMax(h, 1));
}

void GLWidget::update() {

    // Ne pas oublier d'appeler super
    QOpenGLWidget::update();

    _world->updateWorld(1/60.0f);
}

void GLWidget::paintGL() {

    // Dessiner avec O'painGL
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _shaderProgram->bind();

    QOpenGLContext *context = this->context();
    for (auto it : _objectsInWorld) {

        // Il s'agit de generer une matrice Model differente pour chaque
        // objet.
        QMatrix4x4 vMatrix = this->cameraLookAt(_alpha, _beta, _distance);
        QMatrix4x4 mMatrix;
        mMatrix.setToIdentity();

        AbstractBody *pbody = it.first;
        mMatrix.translate(
                pbody->getPosition()[0],
                pbody->getPosition()[1],
                pbody->getPosition()[2]);

        // Le corps a une rotation si c'est un solide
        if (Solid *solid = dynamic_cast<Solid *>(pbody)) {

            double rx = solid->getAngularPosition()[0];
            double ry = solid->getAngularPosition()[1];
            double rz = solid->getAngularPosition()[2];

            mMatrix.rotate(rx, 1, 0, 0);
            mMatrix.rotate(ry, 0, 1, 0);
            mMatrix.rotate(rz, 0, 0, 1);
        }

        // On passe une matrice differente a chaque objet:
        _shaderProgram->setUniformValue("mvpMatrix", _pMatrix * vMatrix * mMatrix);

        it.second->draw(context);
    }

    _shaderProgram->release();
}

QMatrix4x4 GLWidget::cameraLookAt(double a, double b, double d) {

    QMatrix4x4 cameraMatrix;
    cameraMatrix.setToIdentity();

    QMatrix4x4 cameraTransformation;
    cameraTransformation.rotate(a, 0, 1, 0);
    cameraTransformation.rotate(b, 1, 0, 0);

    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, d);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);
    cameraMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    return cameraMatrix;
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

void GLWidget::mousePressEvent(QMouseEvent *event) {

    _lastMousePosition = event->pos();
    event->accept();
}


void GLWidget::mouseMoveEvent(QMouseEvent *event) {

    int deltaX = event->x() - _lastMousePosition.x();
    int deltaY = event->y() - _lastMousePosition.y();

    if (event->buttons() & Qt::LeftButton) {

        _alpha -= deltaX;
        while (_alpha < 0)
            _alpha += 360;
        while (_alpha >= 360)
            _alpha -= 360;

        _beta -= deltaY;
        if (_beta < -90)
            _beta = -90;
        if (_beta > 90)
            _beta = 90;

        this->paintGL();
    }

    _lastMousePosition = event->pos();

    event->accept();
}

void GLWidget::wheelEvent(QWheelEvent *event) {

    int delta = event->delta();

    if (event->orientation() == Qt::Vertical) {

        if (delta < 0)
            _distance *= 1.1;
        else if (delta > 0)
            _distance *= 0.9;

        this->paintGL();
    }

    event->accept();
}

void GLWidget::keyPressEvent(QKeyEvent *event) {

    if (event->key() == Qt::Key_Space) {
        // [okis goes here]
    }
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
