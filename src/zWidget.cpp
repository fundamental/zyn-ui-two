#include "zWidget.h"

#include "../nanovg/nanovg.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>

NVGcontext *initVG();

zWidget::zWidget(QQuickItem *parent)
    :QQuickItem(parent), m_zscale(1.0), m_zaspect(1.0),
     m_label(""), m_zexpandable(false)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
}

void zWidget::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, SIGNAL(beforeSynchronizing()),
                this, SLOT(handleSync()), Qt::DirectConnection);
        win->setClearBeforeRendering(false);
    }
}

void zWidget::handleSync()
{
    //printf("%p %p %p %d\n", this, parentItem(), window()->contentItem(),
    //        parentItem()-window()->contentItem());
    if(parentItem() == window()->contentItem())  {
        abstractPaint();
    }
}

static void draw(QObject *item)
{
    QQuickItem *quick = dynamic_cast<QQuickItem*>(item);
    if(!quick && !item->children().isEmpty())
        ;
    zWidget *X = dynamic_cast<zWidget*>(item);
    if(X)
        X->abstractPaint();
    if(quick) {
        const QList<QQuickItem*> children = quick->childItems();
        size_t N = children.size();
        for(unsigned i=0; i<N; ++i)
            draw(children[i]);
    }
}

void zWidget::abstractPaint()
{
    static long drawCount = 0;
    drawCount++;

    auto pos = mapToItem(window()->contentItem(), QPointF(0,0));
    float yy = (window()->contentItem()->height()-height())-pos.y();
    glViewport(pos.x(), yy, width(), height());
    if(parentItem() == window()->contentItem()) {
        glClearColor(0x06/255.0, 0x27/255.0, 0x37/255.0, 1.0f);
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    }

    printf("draw %ld: '%s'{%s}(%f,%f,%f,%f)\n", drawCount,
            metaObject()->className(), m_label.toLatin1().data(),
            pos.x(),yy,width(),height());
    NVGcontext *vg = (NVGcontext*)initVG();
    glViewport(pos.x(), yy, width(), height());
    nvgBeginFrame(vg, width(), height(), 1.0);
    paint(vg);
    nvgEndFrame(vg);

    if(parentItem() == window()->contentItem()) {
        size_t N = childItems().size();
        for(unsigned i=0; i<N; ++i)
            draw(childItems()[i]);
    }
}

std::string zWidget::getLabel() const
{
    if(m_label.isEmpty())
        return "LABEL";
    std::string tmp = m_label.toLatin1().data();
    std::string result;
    const char *label = tmp.c_str();
    bool wasSpace = false;
    while(*label) {
        char c = *label;
        if((c==' ' && !wasSpace) || c != ' ')
            result += (char) toupper(c);
        wasSpace = c==' ';
        label++;
    }
    return result;
}
