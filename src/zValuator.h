#pragma once
#include <QtQuick/QQuickItem>

class zValuator:public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t MEMBER m_t NOTIFY tChanged);

public:
    zValuator();
    virtual ~zValuator(){};

signals:
    void tChanged();

public slots:
    void update();
    virtual void paint()=0;
    void handleWindowChanged(QQuickWindow *win);

protected:
    qreal m_t;
};
