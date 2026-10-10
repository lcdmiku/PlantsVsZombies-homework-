#include "selectplant.h"

SelectPlant::SelectPlant(QGraphicsObject *parent) : CardsContainer(":/res/GameRes/images/selectPlantBg.png",parent)
{

}

SelectPlant::~SelectPlant() {
    // 析构函数实现
}


//添加卡片
void SelectPlant::addCard(QString plantName){
    if(cards.size()<50)
    {
        Card *newCard = new Card(this,plantName);
        newCard->setZValue(1);
        int cardNum = cards.size();
        newCard->setScale(0.54);
        newCard->setPos(10 + (cardNum%7)*55, 35+(cardNum/7)*80);
        connect(newCard,&Card::cardPress,this,[=](Card* card){
            newCard->setUnselectable();
            emit SelectPlant::cardPress(card);
        });//处理卡片选择

        cards.push_back(newCard);
    }
}

void SelectPlant::reSet(){
    for(auto card:std::as_const(cards)){
        card->setSelectable();
    }
}
