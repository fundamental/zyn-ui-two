#pragma once
#include "zLayout.h"
#include <cassert>

class zModule:public zLayout
{
    Q_OBJECT
    Q_PROPERTY(bool editable MEMBER m_editable);
    Q_PROPERTY(bool copyable MEMBER m_copyable);
public:
    zModule() {
        connect(this, SIGNAL(childrenChanged()), this, SLOT(updateImplicit()));
    }
    virtual void paint(NVGcontext *vg);
public slots:
    void updateImplicit();
protected:
    float layoutY() const {return 0.2f*height();};
    float layoutH() const {return height()*0.8f;};
private:
    bool m_editable;
    bool m_copyable;
};
