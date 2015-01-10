#include "zBlank.h"
#include <GL/gl.h>
#include <QtQuick/qquickwindow.h>

zBlank::zBlank()
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
}

void zBlank::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        //connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(abstractPaint()), Qt::DirectConnection);
        win->setClearBeforeRendering(false);
    }
}

void draw(QObject *item)
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

void zBlank::abstractPaint()
{
    auto pos = mapToItem(window()->contentItem(), QPointF(0,0));
    float yy = (window()->contentItem()->height()-height())-pos.y();
    glViewport(pos.x(), yy, width(), height());
    glClearColor(0x06/255.0, 0x27/255.0, 0x37/255.0, 1.0f);
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    
    size_t N = children().size();
    for(unsigned i=0; i<N; ++i)
        draw(children()[i]);
}

