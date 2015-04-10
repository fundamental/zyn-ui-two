#include "zWidget.h"

#include "../nanovg/nanovg.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>

NVGcontext *initVG();

zWidget::zWidget(QQuickItem *parent)
    :QQuickItem(parent), m_zscale(1.0), m_zaspect(1.0),
     m_label(""), m_zexpandable(false), m_damage(0,0,0,0)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)),
            this, SLOT(handleWindowChanged(QQuickWindow*)));

    connect(this, SIGNAL(xChanged()),
            this, SLOT(abstractDamageLayout()));
    connect(this, SIGNAL(yChanged()),
            this, SLOT(abstractDamageLayout()));
    connect(this, SIGNAL(heightChanged()),
            this, SLOT(abstractDamageLayout()));
    connect(this, SIGNAL(widthChanged()),
            this, SLOT(abstractDamageLayout()));
    connect(this, SIGNAL(childrenChanged()),
            this, SLOT(abstractDamageLayout()));
}

void zWidget::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, SIGNAL(beforeSynchronizing()),
                this, SLOT(handleSync()), Qt::DirectConnection);
        win->setClearBeforeRendering(false);
    }
}

void zWidget::handleSync()
{
    static long syncCount = 0;
    syncCount++;
    //printf("handleSync %ld: '%s'{%s}(%s)\n", syncCount,
    //        metaObject()->className(), m_label.toLatin1().data(),
    //        parentItem()->metaObject()->className());
    //printf("%p %p %p %d\n", this, parentItem(), window()->contentItem(),
    //        parentItem()-window()->contentItem());
    if(parentItem() == window()->contentItem())  {
        //only redraw damaged area when possible
        if(m_damage.isValid() && !m_damage.isEmpty()) {
            abstractPaint(m_damage);
        } else
            abstractPaint(QRectF(0,0,-1,-1));
        m_damage = QRectF(0,0,0,0);
    }
}

static void draw(QObject *item, QRectF damage=QRectF(0,0,-1,-1))
{
    QQuickItem *quick = dynamic_cast<QQuickItem*>(item);
    if(!quick && !item->children().isEmpty())
        ;
    zWidget *X = dynamic_cast<zWidget*>(item);
    if(X)
        X->abstractPaint(damage);
    if(quick) {
        const QList<QQuickItem*> children = quick->childItems();
        size_t N = children.size();
        for(unsigned i=0; i<N; ++i)
            draw(children[i],damage);
    }
}

void zWidget::abstractPaint(QRectF damage)
{
    static long drawCount = 0;

    auto pos = mapToItem(window()->contentItem(), QPointF(0,0));
    float yy = (window()->contentItem()->height()-height())-pos.y();
    auto bb = QRectF(pos.x(), pos.y(), width(), height());
    if(damage.isValid() && !damage.intersects(bb)) {
        return;
    }

    drawCount++;

    glViewport(pos.x(), yy, width(), height());

    printf("draw %ld: '%s'{%s}(%f,%f,%f,%f)\n", drawCount,
            metaObject()->className(), m_label.toLatin1().data(),
            pos.x(),yy,width(),height());
    NVGcontext *vg = (NVGcontext*)initVG();
    glViewport(pos.x(), yy, width(), height());
    nvgBeginFrame(vg, width(), height(), 1.0);
    if(damage.isValid()) {
        auto dmg = mapRectFromItem(window()->contentItem(), damage);

        float sx = dmg.x();
        float sy = dmg.y();
        nvgScissor(vg, sx, sy, dmg.width(), dmg.height());
    }
    paint(vg);
    nvgEndFrame(vg);

    if(parentItem() == window()->contentItem()) {
        size_t N = childItems().size();
        for(unsigned i=0; i<N; ++i)
            draw(childItems()[i], damage);
    }
}

std::string zWidget::getLabel() const
{
    if(m_label.isEmpty())
        return "LABEL";
    std::string tmp = m_label.toLatin1().data();
    std::string result;
    const char *label = tmp.c_str();
    bool wasSpace = false;
    while(*label) {
        char c = *label;
        if((c==' ' && !wasSpace) || c != ' ')
            result += (char) toupper(c);
        wasSpace = c==' ';
        label++;
    }
    return result;
}
void zWidget::tryDamage(QQuickItem *w)
{
    auto pos = w->mapToItem(window()->contentItem(), QPointF(0,0));
    float yy = pos.y();//(window()->contentItem()->height()-height())-pos.y();
    m_damage = QRectF(pos.x(), yy, w->width(), w->height());
}
    
BBox &zWidget::layoutSubProblems(LayoutProblem &prob, BBox &parent)
{
    bbox.parent = &parent;
    bbox.clear();

    if(!m_zexpandable)
        bbox.h = m_zaspect*bbox.w;

    return bbox;
}

void setBounds(QQuickItem &o, const BBox &box)
{
    setBounds(o, box.x.solution, box.y.solution, box.w.solution, box.h.solution);
}
void setBounds(QQuickItem &o, float x, float y, float w, float h)
{
    o.setX(x);
    o.setY(y);
    o.setWidth(w);
    o.setHeight(h);
}
