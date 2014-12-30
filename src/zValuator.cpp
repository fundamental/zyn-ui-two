#include "zValuator.h"
#include <QtQuick/qquickwindow.h>

zValuator::zValuator()
    :m_t(0)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
}

void zValuator::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        //connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(paint()), Qt::DirectConnection);
        win->setClearBeforeRendering(false);
    }
}
