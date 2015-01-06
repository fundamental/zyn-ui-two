#pragma once
#include "zWidget.h"
#include <QStringList>

class zTextList:public zWidget
{
    Q_OBJECT
public:
    zTextList();
    void paint(NVGcontext *vg);
    QStringList strings;
};
