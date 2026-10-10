#ifndef NOMALZOMBIE_H
#define NOMALZOMBIE_H

#include "zombie.h"

class NomalZombie : public Zombie
{
    Q_OBJECT
public:
    explicit NomalZombie();

    ~NomalZombie();

signals:


protected:


    //myobject特征

    //僵尸行为特征
    void Action(Plant *plant) override;
    void setCurrentGif()override;
private slots:
};

#endif // NOMALZOMBIE_H
