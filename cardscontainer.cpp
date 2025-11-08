#include "cardscontainer.h"
#include <QPainter>
#include<QDebug>


CardsContainer::CardsContainer(QString pixmapPath,QGraphicsObject *parent) : QGraphicsObject(parent),pixmapItem(nullptr)
{
    // 创建内部的QPixmapItem
    pixmapItem = new QGraphicsPixmapItem(QPixmap(pixmapPath), this);
    pixmapItem->setZValue(0);
    pixmapItem->setPos(0, 0);
    // 构造函数实现

}
QRectF CardsContainer::boundingRect() const
{
    // 返回包含内部pixmapItem的矩形
    return pixmapItem->boundingRect();
}

void CardsContainer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}

CardsContainer::~CardsContainer() {
    // 析构函数实现
}



//添加卡片
void CardsContainer::addCard(QString plantName){
    Card *newCard = new Card(this,plantName);
    newCard->setZValue(1);
    int cardNum = cards.size();
    newCard->setScale(0.54);
    newCard->setPos(72 + cardNum*55, 5);
    cards.push_back(newCard);
}
//根据索引删除卡片
void CardsContainer::deleteCard(int i){
    if(i>=0 && i<cards.size()){
    delete cards[i];
    cards.erase(cards.begin()+i);
    }
}
//清除卡片
void CardsContainer::clearCards(){
    for(int i=0;i<cards.size();i++){
        delete cards[i];
    }
    cards.clear();
}
