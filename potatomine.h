#ifndef POTATOMINE_H
#define POTATOMINE_H

#include "plant.h"
#include"bullet.h"

class PotatoMine : public Plant
{
    Q_OBJECT

public:
    explicit PotatoMine(QGraphicsObject *parent);
    int hideTime;
    bool hide;

    ~PotatoMine();



protected:
    int power;
    void plantAction()override;
    void setCurrentGif()override;

    QPainterPath shape() const override;


};


#endif // POTATOMINE_H
