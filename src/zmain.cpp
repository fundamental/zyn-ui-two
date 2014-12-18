#include <QApplication>
#include <QtDeclarative/QDeclarativeItem>
#include <QtDeclarative/QDeclarativeView>
#include "zKnob.h"
#include <cassert>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<zKnob>("MDEPlugins", 1, 0, "MDE");
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
    view.setSource(QUrl::fromLocalFile("test.qml"));
    view.show();
    //GLWidget gl(format);
    //gl.show();
    return app.exec();
}
