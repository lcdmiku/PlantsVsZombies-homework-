#ifndef SUNFLOWER_H
#define SUNFLOWER_H

#include "plant.h"


class SunFlower : public Plant
{
    Q_OBJECT
    QTimer *timer;
    int sunlightInterval;
public:
    explicit SunFlower(QGraphicsObject *parent);
    ~SunFlower();
signals:
    void sunlightProduce(SunLight *sunlight);
protected:
    void plantAction()override;
};

#endif // SUNFLOWER_H
