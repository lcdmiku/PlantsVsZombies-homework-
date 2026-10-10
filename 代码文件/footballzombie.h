#ifndef FOOTBALLZOMBIE_H
#define FOOTBALLZOMBIE_H

#include "zombie.h"


class FootballZombie : public Zombie
{
    Q_OBJECT
public:
    explicit FootballZombie();

    ~FootballZombie();

signals:


protected:
    QRectF boundingRect() const override;

    //myobject特征

    //僵尸行为特征
    void Action(Plant *plant) override;
    void setCurrentGif()override;
private slots:

};

#endif // FOOTBALLZOMBIE_H
