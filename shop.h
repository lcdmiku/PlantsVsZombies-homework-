#ifndef SHOP_H
#define SHOP_H

#include<QGraphicsSimpleTextItem>
#include"cardscontainer.h"

class Shop : public CardsContainer
{
    Q_OBJECT
    int totalValue;
    QGraphicsSimpleTextItem *sunlightText;
public:
    explicit Shop(QGraphicsObject *parent = nullptr);

    ~Shop() override;

    //显示阳光值并判断阳光是否足够,重载两份
    bool sunlightValueShow(int sunlightValue,enum PlantType plantType =  PlantType::None);
    bool sunlightValueShow(int sunlightValue);
    //减少阳光值或增加阳光值
    void increaseSunlight(int num);
    void reduceSunlight(int num);
    // 设置阳光值
    void setSun(int val);
    //添加卡片
    void addCard(QString plantName);


signals:
    // 信号声明

};

#endif // SHOP_H
