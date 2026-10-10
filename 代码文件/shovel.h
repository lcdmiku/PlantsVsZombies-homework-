#ifndef SHOVEL_H
#define SHOVEL_H

#include <QGraphicsPixmapItem>

class Shovel : public QGraphicsPixmapItem
{
    bool isSelected;
    QPointF startPos;

    void mousePressEvent(QGraphicsSceneMouseEvent *ev)override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *ev)override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *ev)override;


public:
    Shovel();
    ~Shovel();
    bool getSelected(){return isSelected;}
    QPointF getStartPos(){return startPos;}


};

#endif // SHOVEL_H
