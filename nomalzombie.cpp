#include "nomalzombie.h"
#include<QDebug>

NomalZombie::NomalZombie(QPointF start,QPointF end)
    :Zombie(":/res/GameRes/images/ZombieWalk1.gif",ZombieType::NormalZombie,
             ":/res/GameRes/images/ZombieAttack.gif",
            1000,29,500,
            start,end)//int hp,int speed,int attackpower,
{

}


NomalZombie::~NomalZombie(){

}


//


void NomalZombie::setCurrentGif(){

    if(!attackedPlant){
        double rate = (double)currentHp / getMaxHp();
        if(rate>0.3)
        CurrentGif = ":/res/GameRes/images/ZombieWalk1.gif";
        else {
            CurrentGif = ":/res/GameRes/images/ZombieWalk2.gif";
        }
    }
    Zombie::setCurrentGif();
}

void NomalZombie::Action(Plant *plant){
    Zombie::attack(plant);
}
