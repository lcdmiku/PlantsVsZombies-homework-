#include "screenzombie.h"
#include<QDebug>

ScreenZombie::ScreenZombie(QPointF start,QPointF end)
    :Zombie(":/res/GameRes/images/ScreenZombieWalk.gif",ZombieType::NormalZombie,
             ":/res/GameRes/images/ScreenZombieAttack.gif",
             1000,29,500,
             start,end)//int hp,int speed,int attackpower,
{

}


ScreenZombie::~ScreenZombie(){

}

QRectF ScreenZombie::boundingRect() const {
    QRectF rawRect = Zombie::boundingRect();

    rawRect.setX(-40);
    // rawRect.setRight(150);
    // rawRect.setTop(20);
    return rawRect;
}
//


void ScreenZombie::setCurrentGif(){
    Zombie::setCurrentGif();
    if(!attackedPlant){
            CurrentGif = ":/res/GameRes/images/ScreenZombieWalk.gif";
    }
}

void ScreenZombie::Action(Plant *plant){
    Zombie::attack(plant);
}
