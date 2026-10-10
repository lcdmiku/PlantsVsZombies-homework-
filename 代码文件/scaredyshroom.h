#ifndef SCAREDYSHROOM_H
#define SCAREDYSHROOM_H


#include"plant.h"
class ScaredyShroom : public Plant
{
    Q_OBJECT

public:
    explicit ScaredyShroom(QGraphicsObject *parent);
    ~ScaredyShroom();

protected:
    void plantAction()override;

    int bulletHz;

};


#endif // SCAREDYSHROOM_H
