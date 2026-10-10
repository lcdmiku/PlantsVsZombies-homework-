#ifndef CARDSCONTAINER_H
#define CARDSCONTAINER_H

#include <QGraphicsObject>
#include <QGraphicsPixmapItem>
#include"card.h"
#include<QVector>
#include"myobj.h"

class CardsContainer : public MyObject
{
    Q_OBJECT
protected:
    QGraphicsPixmapItem *pixmapItem;
    QVector<Card*> cards;
public:
    explicit CardsContainer(QString pixmapPath,QGraphicsObject *parent = nullptr);
    // 必须实现的虚函数
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    ~CardsContainer() override;

    //添加卡片
    void addCard(QString plantName);
    //根据索引删除卡片
    void deleteCard(int i);
    //清除卡片
    void clearCards();


signals:
    // 信号声明

};
#endif // CARDSCONTAINER_H
