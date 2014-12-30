#pragma once
#include <QtQuick/QQuickItem>
 
class zDropDown : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int value MEMBER m_value)
 
    public:
        zDropDown();
    public slots:
        void paint();
        void handleWindowChanged(class QQuickWindow *win);

    private:
        int m_value;
};
