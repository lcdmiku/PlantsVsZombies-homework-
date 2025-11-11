#ifndef DOMINATOR_H
#define DOMINATOR_H

#include <QObject>
#include"myobj.h"

enum class PlantType;
//开始写最关键的内容
// :/res/GameRes/images/muliBoki.gif
class Dominator : public MyObject
{
    Q_OBJECT

public:
    Dominator();
    virtual void setCurrentGif();//设计各种情况的图片gif,或许可以利用来进行类继承

    //
    void infoText(QString info);//显示要说的话
    void summonZombies(enum ZombieType zombieType,QPointF pos);//召唤僵尸
    void wipeZombie(QPointF pos);//移除僵尸
    void plant(enum PlantType plantype,QPointF pos);//种植植物
    void uproot(QPointF pos);//铲除植物
    void stealSunlight(int num);//偷取阳光
    void giveSunlight(int num);//给予阳光
    void setCoverage();//类似于护眼模式
    void unPlantabel(QPointF pos,QString path);//使某个地方无法种植,path 是无法种植地方覆盖物的图片路径
    void gameOver();//处理游戏结束结局


};

#endif // DOMINATOR_H
