#include "bucketzombie.h"

BucketZombie::BucketZombie()
    :Zombie(":/res/GameRes/images/BucketZombieWalk.gif",ZombieType::BucketZombie,
             ":/res/GameRes/images/BucketZombieAttack.gif",
             550,20,28)//int hp,int speed,int attackpower,
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

void BucketZombie::beHeated(int power,enum DieType dieType){
    Zombie::beHeated(power,dieType);
    playSoundEffect("qrc:/res/GameRes/audio/shieldhit.wav");//play hetted sound
}

void BucketZombie::Action(Plant *plant){
    Zombie::attack(plant);
}
