#ifndef GAMELEVELDATA_H
#define GAMELEVELDATA_H
#include <QObject>
#include"myobj.h"
#include <QSettings>
#include <QString>
#include <QVariant>
class Dominator;
class GameScene;
//每个ZombieData代表可能出现的僵尸的信息
class ZombieData
{
public:
    enum ZombieType eName;//储存该关卡出现的僵尸种类
    int power;//存储该僵尸的强度
    QList<int> waveWeight;//按顺序存储每波该僵尸出现权重
};

class GameLevelData
{
public:
    GameLevelData();
    virtual ~GameLevelData() {}

    QString eName, cName;//关卡序号及名称

    QList<QString> pName;//可供选择的植物
    int sunProb;//阳光生成可能性，0-100
    QList<int> mowerRow;//各行小推车情况
    QString backgroundImage;//背景图片路径
    bool hasShovel;//是否有shovel
    int maxSelectedCards;//最大可选择卡片数
    QList<QList<int>> map;//记录地图 0：普通绿地  1：无法种植 2：水池 3：屋顶类需要花盆才能种植的地方 。。。



    int waveNum;//波数
    QList<int> waveDuration;//每一波持续时间/s
    QList<int> largeWaveFlag;//“即将有大批僵尸进攻”（幻听），表示比较大的wave的序号
    QList<int> waveTotWeight;//该波总权重
    void waveTotWeightInit();
    QList<int> waveLimits;//存储每波的强度上限
    // QPair<QList<int>, QList<int> > flagToSumNum;
    QMap<int, std::function<void(GameScene *)> > flagToMonitor;//每波到达后所触发的函数

    //

    QList<ZombieData> zombieData;//各类僵尸信息

    QString backgroundMusic;//背景音乐路径
    //函数
    //在僵尸池里抽取僵尸
    QList<ZombieType> zombieExtract(int currWave);

    virtual void dominatorAct(GameScene *gameScene);//处理每一波dominator的行动逻辑
    virtual void loadData(GameScene *gameScene);
    virtual void startGame(GameScene *gameScene);
    virtual void endGame(GameScene *gameScene);
};

class GameLevelData_1: public GameLevelData
{

public:
    GameLevelData_1();
};

GameLevelData * GameLevelDataFactory(const QString &eName);


#endif // GAMELEVELDATA_H
