#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <QGraphicsSceneMouseEvent>
#include "../nanovg/nanovg.h"
#define NANOVG_GL2_IMPLEMENTATION
#include "../nanovg/nanovg_gl.h"
#include "zKnob.h"
#include "../draw.h"
zKnob::zKnob(QDeclarativeItem *parent)
{
     setFlag(QGraphicsItem::ItemHasNoContents, false);
     setKeepMouseGrab(true);
     m_value = 0.1;
     //setMouseTracking(true); 
}
NVGcontext *initVG()
{
    static NVGcontext *vg = NULL;
    //vg = NULL;
    if(!vg) {
        printf("NanoVG Initializing...\n");
        glewExperimental = GL_TRUE;
        if(glewInit() != GLEW_OK) {
            fprintf(stderr, "Failed to start GLEW\n");
            exit(1);
        }
        printf("Initializing NanoVG...\n");
        vg = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
        nvgCreateFont(vg, "sans", "Roboto-Regular.ttf");
        nvgCreateFont(vg, "icons", "entypo.ttf");
    }
    return vg;
}
//
void zKnob::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget)
{
    printf("paint(%p,%f,%f,%f,%f)\n", this,
            x(),y(),width(),height());
    painter->beginNativePainting();
    glScissor(x(), widget->window()->height()-height()-y(), width(), height());
    glEnable(GL_SCISSOR_TEST);
    glClearColor(0, 0, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
    NVGcontext *vg = (NVGcontext*)initVG();
    //printf("Painting...\n");
    //drawButton(vg, "banana", 0, 0, 100, 50);
    printf("view(%d vs %d)\n", widget->window()->height()-height()-y(), y());
    glViewport(x(), widget->window()->height()-height()-y(), width(), height());
    nvgBeginFrame(vg, width(), height(), 1.0);
    //drawButton(vg, "banana", 100, 25, 100, 50);
    //drawOptButton(vg, "opt", 200, 25, 100, 50);
    //drawButtonGrid(vg, 4,4, 300, 0, 100, 100);
    drawAltDial(vg, m_value, 0, 0, width(),height());//400, 0, 100, 100);
    //drawGrid(vg, 8, 8, 500, 0, 200, 200);
    //drawSin(vg,  500, 0, 200, 200);
    //	nvgBeginPath(vg);
    //    float cx = 40.0, cy = 40.0, h = 10.0;
    //    nvgArc(vg, cx, cy, 0.4*h, 0, 1.0/2.0*M_PI, 1);
    //    nvgArc(vg, cx, cy, 0.2*h, 1.0/2.0*M_PI, 0, 2);
    //    nvgClosePath(vg);
    //	nvgFillColor(vg, nvgRGBA(0x11,0x45,0x75,255));
    //	nvgFill(vg);
    nvgEndFrame(vg);
    
//    auto vg = initVG();
//    painter->beginNativePainting();
//    glViewport(0, 0, width(), height());
//    glLoadIdentity();
//    glClearColor(0, 0.2f, 0, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
//    printf("Painting...\n");
//    drawButton(vg, "banana", 100, 25, 100, 50);
//    //glColor3ub(0,0,255);
//    //glVertex2d(0, 0);
//    //glVertex2d(0, height());
//    //glColor3ub(255,0,0);
//    //glVertex2d(width(), height());
//    //glVertex2d(width(), 0);
//    nvgEndFrame(vg);
//    glBegin(GL_QUADS);
//    glEnd();
//    painter->endNativePainting();
//    glBindTexture(GL_TEXTURE_2D, 0);
}

void zKnob::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
//void zKnob::mouseMoveEvent(QMouseEvent *event)
{
    auto p = event->pos();
    printf("mouse: %f %f\n",p.x(), p.y());
}
        
bool zKnob::sceneEvent ( QEvent * event )
{
    printf("EVENT...\n");
    return false;

}

 #include <QtGui>
// #include <QtOpenGL>

 #include <math.h>

// #include "glwidget.h"
// #include "qtlogo.h"

//#ifndef GL_MULTISAMPLE
//#define GL_MULTISAMPLE  0x809D
//#endif
//
//    GLWidget::GLWidget(QWidget *parent)
//: QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
//{
//    logo = 0;
//    xRot = 0;
//    yRot = 0;
//    zRot = 0;
//
//    qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
//    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
//}
//
//GLWidget::GLWidget(QGLFormat format, QWidget *parent)
//    : QGLWidget(format, parent)
//{
//}
//
//GLWidget::~GLWidget()
//{
//}
//
//QSize GLWidget::minimumSizeHint() const
//{
//    return QSize(50, 50);
//}
//
//QSize GLWidget::sizeHint() const
//{
//    return QSize(400, 400);
//}
//
//static void qNormalizeAngle(int &angle)
//{
//    while (angle < 0)
//        angle += 360 * 16;
//    while (angle > 360 * 16)
//        angle -= 360 * 16;
//}
//
////void GLWidget::setXRotation(int angle)
////{
////    qNormalizeAngle(angle);
////    if (angle != xRot) {
////        xRot = angle;
////        emit xRotationChanged(angle);
////        updateGL();
////    }
////}
//
////void GLWidget::setYRotation(int angle)
////{
////    qNormalizeAngle(angle);
////    if (angle != yRot) {
////        yRot = angle;
////        emit yRotationChanged(angle);
////        updateGL();
////    }
////}
//
////void GLWidget::setZRotation(int angle)
////{
////    qNormalizeAngle(angle);
////    if (angle != zRot) {
////        zRot = angle;
////        emit zRotationChanged(angle);
////        updateGL();
////    }
////}
//
//void GLWidget::initializeGL()
//{
//    printf("InitializeGL()\n");
//    makeCurrent();
//    //glViewport(0, 0, width(), height());
//    //glClearColor(0x06/255.0, 0x27/255.0, 0x37/255.0, 1.0f);
//    glClearColor(0, 0, 0.0, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
//
//    //glDisable(GL_STENCIL_TEST);
//    //glStencilMask(0x0);
//    //glEnable(GL_STENCIL_TEST);
//    //glStencilMask(0x1);
//    //qglClearColor(qtPurple.dark());
//
//    //    logo = new QtLogo(this, 64);
//    //    logo->setColor(qtGreen.dark());
//
//    //glEnable(GL_DEPTH_TEST);
//    //glEnable(GL_CULL_FACE);
//    //glShadeModel(GL_SMOOTH);
//    //glEnable(GL_LIGHTING);
//    //glEnable(GL_LIGHT0);
//    //glEnable(GL_MULTISAMPLE);
//    //static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
//    //glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
//    vg_ = initVG();
//}
//
//void GLWidget::paintGL()
//{
//    glInit();
//    printf("PaintGL()\n");
//    NVGcontext *vg = (NVGcontext*)vg_;
//    printf("Painting...\n");
//    //drawButton(vg, "banana", 0, 0, 100, 50);
//    glViewport(0, 0, width(), height());
//    nvgBeginFrame(vg, width(), height(), 1.0);
//    drawButton(vg, "banana", 100, 25, 100, 50);
//    drawOptButton(vg, "opt", 200, 25, 100, 50);
//    drawButtonGrid(vg, 4,4, 300, 0, 100, 100);
//    drawAltDial(vg, 400, 0, 100, 100);
//    drawGrid(vg, 8, 8, 500, 0, 200, 200);
//    drawSin(vg,  500, 0, 200, 200);
//    //	nvgBeginPath(vg);
//    //    float cx = 40.0, cy = 40.0, h = 10.0;
//    //    nvgArc(vg, cx, cy, 0.4*h, 0, 1.0/2.0*M_PI, 1);
//    //    nvgArc(vg, cx, cy, 0.2*h, 1.0/2.0*M_PI, 0, 2);
//    //    nvgClosePath(vg);
//    //	nvgFillColor(vg, nvgRGBA(0x11,0x45,0x75,255));
//    //	nvgFill(vg);
//    nvgEndFrame(vg);
//    GLenum err;
//    err = glGetError();
//    if (err != GL_NO_ERROR) {
//        printf("Error %08x after %s\n", err, "something...");
//        return;
//    }
//    //swapBuffers();
//}
//
//void GLWidget::resizeGL(int width, int height)
//{
//    printf("resize = [%d,%d]\n",width, height);
//    paintGL();
//    //int side = qMin(width, height);
//    //glViewport((width - side) / 2, (height - side) / 2, side, side);
//
//    //glMatrixMode(GL_PROJECTION);
//    //glLoadIdentity();
//    //glOrthof(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
//    //glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
//    //glMatrixMode(GL_MODELVIEW);
//}
//
//void GLWidget::mousePressEvent(QMouseEvent *event)
//{
//    lastPos = event->pos();
//}
//
//void GLWidget::mouseMoveEvent(QMouseEvent *event)
//{
//    int dx = event->x() - lastPos.x();
//    int dy = event->y() - lastPos.y();
//
//    //     if (event->buttons() & Qt::LeftButton) {
//    //         setXRotation(xRot + 8 * dy);
//    //         setYRotation(yRot + 8 * dx);
//    //     } else if (event->buttons() & Qt::RightButton) {
//    //         setXRotation(xRot + 8 * dy);
//    //         setZRotation(zRot + 8 * dx);
//    //     }
//    lastPos = event->pos();
//}
