#include "bucketzombie.h"

BucketZombie::BucketZombie(QPointF start,QPointF end)
    :Zombie(":/res/GameRes/images/BucketZombieWalk.gif",ZombieType::BucketZombie,
             ":/res/GameRes/images/BucketZombieAttack.gif",
             2000,29,500,
             start,end)//int hp,int speed,int attackpower,
{

}


BucketZombie::~BucketZombie(){

}

QRectF BucketZombie::boundingRect() const {
    QRectF rawRect = Zombie::boundingRect();

    rawRect.setX(-40);
    // rawRect.setRight(150);
    rawRect.setY(-100);
    return rawRect;
}
//


void BucketZombie::setCurrentGif(){
    Zombie::setCurrentGif();
    if(!attackedPlant){
        CurrentGif = ":/res/GameRes/images/BucketZombieWalk.gif";
    }
}

void BucketZombie::Action(Plant *plant){
    Zombie::attack(plant);
}
