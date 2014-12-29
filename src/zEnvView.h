#pragma once
#include <QtQuick/QQuickItem>

class zEnvView:public QQuickItem
{
    Q_OBJECT
public:
    zEnvView();

public slots:
    void paint();
    void handleWindowChanged(QQuickWindow *win);
private:
    float m_data[16];
};
