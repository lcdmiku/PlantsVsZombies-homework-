#ifndef BULLET_H
#define BULLET_H
#include"myobj.h"
#include "zombie.h"

class Zombie;
class Bullet : public MyObject
{
    Q_OBJECT
    int power;//子弹威力
    // QTimer *moveTimer;
    int Hz;
    double speed;
    //

public:
    explicit Bullet(QString objPath,int power);

    int getHz(){return Hz;}
    double getSpeed(){return speed;}
    void setSpeed(double speed){speed = speed;}
    void setBombGif(QString path){bomb = path;}
    ~Bullet();
    static int bulletNum;
    static int delNum;

protected:
    QString bomb;//子弹打中敌人动画gif路径
    QString bombSound;
    virtual void setCurrentGif(){if(isDead && !bomb.isEmpty())CurrentGif = bomb;}
    virtual void DealZombie(Zombie *zombie);
    virtual void dealBomb();//处理子弹打中

signals:


};
#endif // BULLET_H
