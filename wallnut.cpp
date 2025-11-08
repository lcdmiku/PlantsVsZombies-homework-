#include "wallnut.h"

WallNut::WallNut(QGraphicsObject *parent)
    :Plant(parent,":/res/GameRes/images/WallNut.gif",6000)//cost=100,hp=4000
{

}

void WallNut::setCurrentGif(){
    int MaxHp = this->getMaxHp();
    double rate = (double)currentHp / MaxHp;

    if(rate<0.3)CurrentGif = ":/res/GameRes/images/WallNut2.gif";
    else if(rate<0.6)CurrentGif = ":/res/GameRes/images/WallNut1.gif";
}

void WallNut::plantAction(){

}

