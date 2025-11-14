#ifndef DOMINATOR_H
#define DOMINATOR_H

#include <QObject>
#include"myobj.h"
#include "dialogbox.h"
#include"coordinate.h"
enum class PlantType;
//开始写最关键的内容
// :/res/GameRes/images/muliBoki.gif
class Dominator : public MyObject
{
    Q_OBJECT
    int speed;//移动速度
    qreal speedRate;//当前配速与speed的比值，默认为1
protected:
    DialogBox *dialog;
    QGraphicsPixmapItem *attachment;//附带图片
    Coordinate coordinate;

public:
    Dominator();
    virtual void setCurrentGif();//设计各种情况的图片gif,或许可以利用来进行类继承

    //
    void showUp();//出场画面
    void setDialog(QString info,QVector<QString> btnStrs);//显示要说的话
    void ZombieGenerate(ZombieType zombieType,int row,int x);//召唤僵尸
    void wipeZombie(QPointF pos,int num=1);//移除僵尸,默认移除1个
    void plant(enum PlantType plantype,int r,int c);//种植植物
    void uproot(int r,int c);//铲除植物
    void stealSunlight(int num);//偷取阳光
    void stealSunlight(int num,int eachVal);//有动画效果，生成num个阳光，每个阳光的值为-eachVal
    void giveSunlight(int num);//给予阳光
    void giveSunlight(int num,int eachVal);//有动画效果，生成num个阳光，每个阳光的值为eachVal
    void setCoverage();//类似于护眼模式
    void unPlantabel(QPointF pos,QString path);//使某个地方无法种植,path 是无法种植地方覆盖物的图片路径
    void gameOver();//处理游戏结束结局

    //模拟用户行为
    void simulateDrag(QPointF pos,QPointF toPos,QPixmap pixmap);//模拟拖拽


};

#endif // DOMINATOR_H
