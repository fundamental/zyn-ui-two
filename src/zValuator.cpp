#include "zValuator.h"
#include <QtQuick/qquickwindow.h>

zValuator::zValuator()
    :m_t(0)
{
    connect(this, SIGNAL(tChanged()), this, SLOT(update()));
}

void zValuator::update()
{
    if(window())
        window()->update();
}
