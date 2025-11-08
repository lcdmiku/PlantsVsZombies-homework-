#ifndef PEASHOOTER_H
#define PEASHOOTER_H

#include "plant.h"


class Peashooter : public Plant
{
    Q_OBJECT

public:
    explicit Peashooter(QGraphicsObject *parent,QTimer *bulletTimer);
    ~Peashooter();

protected:
    void plantAction()override;
    QTimer *m_bulletTimer;
    int bulletHz;

};

#endif // PEASHOOTER_H
