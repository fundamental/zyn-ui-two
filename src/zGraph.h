#pragma once
#include <QtDeclarative/QDeclarativeItem>
#include <QtOpenGL/QGLWidget>
 
class zGraph : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(float value READ value WRITE setValue)
 
    public:
        zGraph(QDeclarativeItem *parent = 0);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget = 0);
        void  mouseMoveEvent (QGraphicsSceneMouseEvent *event) override;
        bool sceneEvent(QEvent *event) override;
        float value() const {return m_value;}
        void  setValue(float v) {m_value=v;
        this->update(boundingRect());}

    private:
        float m_value;
};
