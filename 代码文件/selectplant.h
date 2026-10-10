#ifndef SELECTPLANT_H
#define SELECTPLANT_H

#include "cardscontainer.h"


class SelectPlant : public CardsContainer
{
    Q_OBJECT

public:
    explicit SelectPlant(QGraphicsObject *parent = nullptr);


    ~SelectPlant() override;
    //添加卡片
    void addCard(QString plantName);
    void reSet();//当shop清空时需要重新选择植物


signals:
    // 信号声明
    void cardPress(Card *card);

};
#endif // SELECTPLANT_H
