#include <QtGui/QApplication>
#include <QtDeclarative/QDeclarativeItem>
#include <QtDeclarative/QDeclarativeView>
#include <cassert>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../nanovg/nanovg.h"
#define NANOVG_GL2_IMPLEMENTATION
#include "../nanovg/nanovg_gl.h"
#include "zKnob.h"
#include "zGraph.h"


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
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<zKnob>("ZynAddSubFX", 1, 0, "Knob");
    qmlRegisterType<zGraph>("ZynAddSubFX", 1, 0, "Graph");
    QDeclarativeView view;
    //QGLFormat format;
    //format.setVersion(2,1);
    //format.setSampleBuffers(false);
    //format.setStencil(true);
    //format.setStereo(false);
    //format.setDoubleBuffer(true);
    //format.setSwapInterval(0);
    //assert(format.stencil());
    QGLWidget *glWidget = new QGLWidget();//format);
    view.setViewport(glWidget);
    view.setResizeMode(QDeclarativeView::SizeRootObjectToView);
    //view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view.setSource(QUrl("qrc:/qml/test.qml"));
    view.show();
    //GLWidget gl(format);
    //gl.show();
    return app.exec();
}
