#include "conezombie.h"

ConeZombie::ConeZombie()
    :Zombie(":/res/GameRes/images/ConeZombieWalk.gif",ZombieType::ConeZombie,
             ":/res/GameRes/images/ConeZombieAttack.gif",
             250,29,20)//int hp,int speed,int attackpower,
{

}


ConeZombie::~ConeZombie(){

}

QRectF ConeZombie::boundingRect() const {
    QRectF rawRect = Zombie::boundingRect();

    rawRect.setX(-10);
    rawRect.setRight(90);
    rawRect.setY(-75);
    return rawRect;
}
//
// 碰撞区域（仅躯干部分）
QPainterPath ConeZombie::shape() const {
    QPainterPath path;
    path.addRect(0,-10,5,80);


    return path;
}

void ConeZombie::setCurrentGif(){
    Zombie::setCurrentGif();
    if(!attackedPlant){
        CurrentGif = ":/res/GameRes/images/ConeZombieWalk.gif";
    }
}

void ConeZombie::Action(Plant *plant){
    Zombie::attack(plant);
}
