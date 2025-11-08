#ifndef SCREENZOMBIE_H
#define SCREENZOMBIE_H

#include "zombie.h"

class ScreenZombie : public Zombie
{
    Q_OBJECT
public:
    explicit ScreenZombie(QPointF start,QPointF end);

    ~ScreenZombie();

signals:


protected:
    QRectF boundingRect() const override;

    //myobject特征

    //僵尸行为特征
    void Action(Plant *plant) override;
    void setCurrentGif()override;
private slots:

};
#endif // SCREENZOMBIE_H
