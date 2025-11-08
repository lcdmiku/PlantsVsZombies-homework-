#ifndef SNOWPEASHOOTER_H
#define SNOWPEASHOOTER_H

#include "plant.h"


class SnowPeashooter : public Plant
{
    Q_OBJECT

public:
    explicit SnowPeashooter(QGraphicsObject *parent,QTimer *bulletTimer);
    ~SnowPeashooter();

protected:
    void plantAction()override;
    QTimer *m_bulletTimer;
    int bulletHz;

};
#endif // SNOWPEASHOOTER_H
