#pragma once
#include <QtQuick/QQuickItem>

class zValuator:public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t MEMBER m_t);

public:
    zValuator();
    virtual ~zValuator(){};

public slots:
    virtual void paint()=0;
    void handleWindowChanged(QQuickWindow *win);

private:
    qreal m_t;
};
