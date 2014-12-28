#pragma once
#include <QtQuick/QQuickItem>

class zKnob:public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)

public:
    zKnob();

    qreal t() const { return m_t; }
    void setT(qreal t);

signals:
    void tChanged();

public slots:
    void paint();
    void handleWindowChanged(QQuickWindow *win);

private:
    qreal m_t;
};
