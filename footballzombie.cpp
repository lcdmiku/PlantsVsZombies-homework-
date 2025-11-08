#include "footballzombie.h"

FootballZombie::FootballZombie(QPointF start,QPointF end)
    :Zombie(":/res/GameRes/images/FootballZombieWalk.gif",ZombieType::FootballZombie,
             ":/res/GameRes/images/FootballZombieAttack.gif",
             2000,40,500,
             start,end)//int hp,int speed,int attackpower,
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
    if(!attackedPlant){
        CurrentGif = ":/res/GameRes/images/FootballZombieWalk.gif";
    }
}

void FootballZombie::Action(Plant *plant){
    Zombie::attack(plant);
}
