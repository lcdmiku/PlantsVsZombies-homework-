#ifndef BUCKETZOMBIE_H
#define BUCKETZOMBIE_H

#include "zombie.h"

class BucketZombie : public Zombie
{
    Q_OBJECT
public:
    explicit BucketZombie(QPointF start,QPointF end);

    ~BucketZombie();

signals:


protected:
    QRectF boundingRect() const override;

    //myobject特征

    //僵尸行为特征
    void Action(Plant *plant) override;
    void setCurrentGif()override;
private slots:

};

#endif // BUCKETZOMBIE_H
