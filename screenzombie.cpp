#include "screenzombie.h"
#include<QDebug>

ScreenZombie::ScreenZombie()
    :Zombie(":/res/GameRes/images/ScreenZombieWalk.gif",ZombieType::NormalZombie,
             ":/res/GameRes/images/ScreenZombieAttack.gif",
             350,29,30
             )//int hp,int speed,int attackpower,
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

void ScreenZombie::beHeated(int power,enum DieType dieType){
    Zombie::beHeated(power,dieType);
    playSoundEffect("qrc:/res/GameRes/audio/shieldhit2.wav");
}

void ScreenZombie::Action(Plant *plant){
    Zombie::attack(plant);
}
