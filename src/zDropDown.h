#pragma once
#include "zWidget.h"
 
class zDropDown:public zWidget
{
    Q_OBJECT
    Q_PROPERTY(int value MEMBER m_value)
 
    public:
        zDropDown();
    public:
        void paint(NVGcontext *vg);
    private:
        int m_value;
};
