#ifndef PLANT_H
#define PLANT_H
#include "myobj.h"
#include"shop.h"
#include"sunlight.h"

class Zombie;

class Plant : public MyObject
{
    Q_OBJECT
    int hp;//hp
    int cost;
public:
    explicit Plant(QGraphicsObject *parent,QString objPath,int duration = 1000);

    //在敌方中使用
    virtual void Attacted(int hurt);
    virtual void DealDead();
    ~Plant();

signals:
    void findZombie(MyObject *zombie);
    void dead();

protected:
    int currentHp;//
    QGraphicsItem *m_detectArea;
    QTimer *m_detectTimer;
    //植物行为特征
    virtual void plantAction() = 0;

    //供派生类访问值
    int getMaxHp(){return hp;}
    void setUneatable(){currentHp = 999999;}//部分植物无法吃掉

private slots:

};

#endif // PLANT_H
