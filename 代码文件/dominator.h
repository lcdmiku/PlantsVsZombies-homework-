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

    //用于创建不同关卡的dominator行为逻辑,只在初始化时使用
    virtual void waveEvent();//响应波次到达
    virtual void btnEvent();//响应按键event
    bool eventInit;

protected:
    DialogBox *dialog;
    QGraphicsPixmapItem *attachment;//附带图片
    Coordinate coordinate;

private:
    bool eyeProtectionOn = false;
    QGraphicsRectItem* eyeProtectionLayer = nullptr;

    PlantType sacrifiedPlant=PlantType::PEASHOOTER;
    QString sacrifiedPlantName = QString("豌豆射手");

public:
    //get
    Dominator(const QString& Path = QString(":/res/GameRes/images/scarysan_small.png"));
    virtual void setCurrentGif();//设计各种情况的图片gif,或许可以利用来进行类继承
    void initEvent();//事件初始化
    //
    void showUp();//出场画面
    void setDialog(QString info,const QVector<QString>& btnStrs = QVector<QString>(),const QVector<int>& btnIds = QVector<int>());//显示要说的话
    void hideDialog();//隐藏对话框
    void ZombieGenerate(ZombieType zombieType,int row,int x);//召唤僵尸
    void ZombieGenerate(QList<QPointF> poses);//召唤僵尸
    void wipeZombie(QPointF pos,int num=1);//移除僵尸,默认移除1个
    void wipeZombie(QList<QPointF> poses);//多位置移除僵尸
    bool plant(enum PlantType plantype,int r,int c);//种植植物
    bool uproot(int r,int c);//铲除植物,bool 返回是否成功
    void stealSunlight(int num);//偷取阳光
    void stealSunlight(int num,int eachVal);//有动画效果，生成num个阳光，每个阳光的值为-eachVal
    void giveSunlight(int num);//给予阳光
    void giveSunlight(int num,int eachVal);//有动画效果，生成num个阳光，每个阳光的值为eachVal
    void ProtectEyes(); // 护眼模式：屏幕蒙上深黄色薄膜，所有僵尸减速20%
    bool isEyeProtectionOn() const { return eyeProtectionOn; } //看护眼模式是否开启
    void setPlantabel(bool plantable,int r,int c,QString path);//使某个地方无法/可以种植,path 是无法种植地方覆盖物的图片路径
    //小推车
    void releaseMower(int r);
    void addMower(int r);
    //加强植物
    void setPlantSpeedRate(qreal rate,int r,int c);//改变（r,c)位置的植物攻速
    void setPlantPowerRate(qreal rate,int r,int c);//改变（r,c)位置的植物威力
    //献祭植物
    bool sacrifyPlant(PlantType planttype);
    //
    void gameOver();//处理游戏结束结局

    void sacrifyImpactBoss(PlantType planttype);


    //随机游走
    void randomWalk();
    void stopRandomWalk();//停止随机游走

    //长线计划函数和变量，即dominator对游戏出现长期影响
    //每波进行阳光偷取
    int sunlightTax;//每波阳光税，默认为零
    //TODO
    //与boss有关参数


    //辅助函数
    PlantType getSacrificedFromBtnId(int btnId);//通过btnId得到献祭植物
    PlantType getRandomPlantType();//获得随机植物类型
    void resetAttachment(){attachment->setPixmap(QPixmap()); attachment->setPos(0,0);}
    void setAttachment(QString pix,QPointF pos = QPointF(0,0)){attachment->setPos(pos);attachment->setPixmap(QPixmap(pix));}
    void setTemporalPix(QString pixPath,QPointF);


signals:
    void branchTriggered(int id);
    void waveStart(int currWave);
};

#endif // DOMINATOR_H
