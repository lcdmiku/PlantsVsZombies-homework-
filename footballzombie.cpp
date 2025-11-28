#include "footballzombie.h"

FootballZombie::FootballZombie()
    :Zombie(":/res/GameRes/images/FootballZombieWalk.gif",ZombieType::FootballZombie,
             ":/res/GameRes/images/FootballZombieAttack.gif",
             600,45,30
             )//int hp,int speed,int attackpower,
{

}


FootballZombie::~FootballZombie(){

}

QRectF FootballZombie::boundingRect() const {
    QRectF rawRect = Zombie::boundingRect();

    rawRect.setX(-80);
    rawRect.setRight(90);
    rawRect.setY(-90);
    return rawRect;
}
//


void FootballZombie::setCurrentGif(){
    Zombie::setCurrentGif();
    qreal rate = (double)currentHp / getMaxHp();
    if(rate<0.5)attackingGif = ":/res/GameRes/images/Zombies/FootballZombie/FootballZombieOrnLostAttack.gif";
    if(!attackedPlant){
        if(rate<0.5){
            CurrentGif = ":/res/GameRes/images/Zombies/FootballZombie/FootballZombieOrnLost.gif";
        }
        else {
            CurrentGif = ":/res/GameRes/images/FootballZombieWalk.gif";
        }
    }
}

void FootballZombie::Action(Plant *plant){
    Zombie::attack(plant);
}
