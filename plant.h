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
    //给gamescene和dominato等大手用
    void setBulletSpeedRate(qreal rate){bulletSpeedRate = rate;}
    void setBulletPowerRate(qreal rate){bulletPowerRate = rate;}
signals:
    void findZombie(MyObject *zombie);
    void dead();

protected:
    int currentHp;//
    //控制植物生成子弹的速度加成与攻击加成，对有子弹的植物起作用
    qreal bulletSpeedRate;
    qreal bulletPowerRate;

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
