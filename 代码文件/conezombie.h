#ifndef CONEZOMBIE_H
#define CONEZOMBIE_H

#include "zombie.h"

class ConeZombie : public Zombie
{
    Q_OBJECT
public:
    explicit ConeZombie();

    ~ConeZombie();

signals:


protected:
    QRectF boundingRect() const override;

    //myobject特征
    QPainterPath shape() const override;
    //僵尸行为特征
    void Action(Plant *plant) override;
    void setCurrentGif()override;
private slots:

};

#endif // CONEZOMBIE_H
