#ifndef SNOWPEASHOOTER_H
#define SNOWPEASHOOTER_H

#include "plant.h"


class SnowPeashooter : public Plant
{
    Q_OBJECT

public:
    explicit SnowPeashooter(QGraphicsObject *parent);
    ~SnowPeashooter();

protected:
    void plantAction()override;

    int bulletHz;

};
#endif // SNOWPEASHOOTER_H
