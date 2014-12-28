#pragma once
#include <QtQuick/QQuickItem>
 
class zGraph : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal value MEMBER m_value)
 
    public:
        zGraph();
    public slots:
        void paint();
        void handleWindowChanged(class QQuickWindow *win);
        //float value() const {return m_value;}
        //void  setValue(float v) {m_value=v;
        //this->update(boundingRect());}

    private:
        float m_value;
};
