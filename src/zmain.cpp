//#include <QtGui/QApplication>
//#include <QtDeclarative/QDeclarativeItem>
//#include <QtDeclarative/QDeclarativeView>
#include <cassert>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../nanovg/nanovg.h"
#define NANOVG_GL2_IMPLEMENTATION
#include "../nanovg/nanovg_gl.h"
#include "zKnob.h"
#include <QGuiApplication>
#include <QtQuick/QQuickView>
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
    QGuiApplication app(argc, argv);

    qmlRegisterType<zKnob>("ZynAddSubFX", 1, 0, "Knob");
    qmlRegisterType<zGraph>("ZynAddSubFX", 1, 0, "Graph");
    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:/qml/test.qml"));
    view.show();
    return app.exec();
}
