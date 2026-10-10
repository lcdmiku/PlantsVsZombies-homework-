#ifndef WALLNUT_H
#define WALLNUT_H

#include "plant.h"

class WallNut : public Plant
{
    Q_OBJECT
public:
    explicit WallNut(QGraphicsObject *parent);

protected:
    void plantAction()override;
    void setCurrentGif()override;//设计各种情况的图片gif
};
#endif // WALLNUT_H
