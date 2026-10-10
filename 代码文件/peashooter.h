#ifndef PEASHOOTER_H
#define PEASHOOTER_H

#include "plant.h"


class Peashooter : public Plant
{
    Q_OBJECT

public:
    explicit Peashooter(QGraphicsObject *parent);
    ~Peashooter();

protected:
    void plantAction()override;

    int bulletHz;

};

#endif // PEASHOOTER_H
