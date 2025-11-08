#include "shop.h"
Shop::Shop(QGraphicsObject *parent) : CardsContainer(":/res/GameRes/images/Shop.png",parent), totalValue(5000)
{

    sunlightText = new QGraphicsSimpleTextItem(this);
    sunlightText->setText(QString::number(totalValue));
    sunlightText->setFont(QFont("Arial", 10, QFont::Bold));
    sunlightText->setBrush(Qt::black);
    sunlightText->setPos(30,64);
    sunlightText->setZValue(1);
    // 构造函数实现

}

Shop::~Shop() {
    // 析构函数实现
}


bool Shop::sunlightValueShow(int sunlightValue,enum PlantType plantType){
    if(totalValue + sunlightValue>=0)
    {
        totalValue += sunlightValue;
        sunlightText->setText(QString::number(totalValue));
        qDebug()<<QString::number(totalValue)<<' '<<QString::number(sunlightValue);
        update();

        //处理冷却
        if(plantType != PlantType::None){
            auto it = std::find_if(cards.begin(),cards.end(),[=](Card *card){
                return card->getPlantType() == plantType;
            });

            if(it != cards.end()){
                int index = std::distance(cards.begin(),it);
                cards[index]->CoolDown();
            }
        }

        return true;
    }
    return false;

}

bool Shop::sunlightValueShow(int sunlightValue){
    if(totalValue + sunlightValue>=0)
    {
        totalValue += sunlightValue;
        sunlightText->setText(QString::number(totalValue));
        qDebug()<<QString::number(totalValue)<<' '<<QString::number(sunlightValue);
        update();

        return true;
    }
    return false;

}
//添加卡片
void Shop::addCard(QString plantName){
    if(cards.size()<6)
    {
        Card *newCard = new Card(this,plantName);
        newCard->setZValue(1);
        int cardNum = cards.size();
        newCard->setScale(0.54);
        newCard->setPos(72 + cardNum*55, 5);

        cards.push_back(newCard);
    }
}
