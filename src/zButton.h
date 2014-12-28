#pragma once
#include <QtQuick/QQuickItem>

class zButton:public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool t MEMBER m_t)

public:
    zButton();

public slots:
    void paint();
    void handleWindowChanged(QQuickWindow *win);

private:
    bool m_t;
};
