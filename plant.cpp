#include "plant.h"
#include"shop.h"
#include"sunlight.h"

Plant::Plant(QGraphicsObject *parent,QString objPath,int duration)
    :MyObject(parent,objPath,Type::PLANT),hp(duration),m_detectArea(nullptr),m_detectTimer(nullptr)
{
    currentHp = hp;
}

Plant::~Plant(){

}



void Plant::Attacted(int hurt){
        currentHp -= hurt;
        if(currentHp<=0){
            DealDead();
        }
}

void Plant::DealDead(){
    emit dead();
    isDead = true;
    this->deleteLater();
}
