#ifndef TROPHY_H
#define TROPHY_H

#include "myobj.h"
#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>
#include <QPixmap>

class Trophy : public MyObject
{
    Q_OBJECT
    QPixmap pixmap;
public:
    explicit Trophy(QGraphicsObject *parent = nullptr);
    ~Trophy() override;
    
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // TROPHY_H
