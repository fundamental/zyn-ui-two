//#include <QtGui/QApplication>
//#include <QtDeclarative/QDeclarativeItem>
//#include <QtDeclarative/QDeclarativeView>
#include <cassert>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../nanovg/nanovg.h"
#define NANOVG_GL2_IMPLEMENTATION
#include "../nanovg/nanovg_gl.h"
#include <QGuiApplication>
#include <QtQuick/QQuickView>
#include "zKnob.h"
#include "zGraph.h"
#include "zButton.h"
#include "zEnvView.h"
#include "zVSlider.h"
#include "zHSlider.h"
#include "zDropDown.h"
#include "zBlank.h"
#include "zHALayout.h"
#include "zTextList.h"
#include "zPowButton.h"
#include "zBox.h"
#include "zVBar.h"
#include "zPanKnob.h"
#include "zVAltSlider.h"
#include "zLayout.h"
#include "zEQView.h"
#include "zTextField.h"
#include "zModule.h"
#include "zDummy.h"


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
        nvgCreateFont(vg, "sans", "../Roboto-Regular.ttf");
        nvgCreateFont(vg, "icons", "../entypo.ttf");
    }
    return vg;
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<zKnob>("ZynAddSubFX", 1, 0, "Knob");
    qmlRegisterType<zGraph>("ZynAddSubFX", 1, 0, "Graph");
    qmlRegisterType<zButton>("ZynAddSubFX", 1, 0, "Button");
    qmlRegisterType<zEnvView>("ZynAddSubFX", 1, 0, "EnvView");
    qmlRegisterType<zVSlider>("ZynAddSubFX", 1, 0, "VSlider");
    qmlRegisterType<zHSlider>("ZynAddSubFX", 1, 0, "HSlider");
    qmlRegisterType<zDropDown>("ZynAddSubFX", 1, 0, "DropDown");
    qmlRegisterType<zBlank>("ZynAddSubFX", 1, 0, "Blank");
    qmlRegisterType<zHALayout>("ZynAddSubFX", 1, 0, "HALayout");
    qmlRegisterType<zLayout>("ZynAddSubFX", 1, 0, "ZLayout");
    qmlRegisterType<zTextList>("ZynAddSubFX", 1, 0, "TextList");
    qmlRegisterType<zPowButton>("ZynAddSubFX", 1, 0, "PowButton");
    qmlRegisterType<zPowButton>("ZynAddSubFX", 1, 0, "PowerButton");
    qmlRegisterType<zBox>("ZynAddSubFX", 1, 0, "Box");
    qmlRegisterType<zVBar>("ZynAddSubFX", 1, 0, "VBar");
    qmlRegisterType<zPanKnob>("ZynAddSubFX", 1, 0, "PanKnob");
    qmlRegisterType<zVAltSlider>("ZynAddSubFX", 1, 0, "VAltSlider");
    qmlRegisterType<zEQView>("ZynAddSubFX", 1, 0, "EQView");
    qmlRegisterType<zTextField>("ZynAddSubFX", 1, 0, "TextField");
    qmlRegisterType<zModule>("ZynAddSubFX", 1, 0, "Module");

    //Unimplemented
    qmlRegisterType<zDummy>("ZynAddSubFX", 1, 0, "Div");

    const char *element = "qrc:/qml/test.qml";
    if(argc == 2)
        element = argv[1];


    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl(element));
    view.show();
    return app.exec();
}
