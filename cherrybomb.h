#ifndef CHERRYBOMB_H
#define CHERRYBOMB_H

#include "plant.h"
#include"bullet.h"

class CherryBomb : public Plant
{
    Q_OBJECT

public:
    explicit CherryBomb(QGraphicsObject *parent);
    ~CherryBomb();

protected:
    int power;
    void plantAction()override;
    QMetaObject::Connection SecondConn;
    // 碰撞区域
    QPainterPath shape() const override;


};
#endif // CHERRYBOMB_H
