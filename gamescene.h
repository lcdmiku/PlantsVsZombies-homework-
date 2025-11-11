#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QRandomGenerator>
#include "shop.h"
#include"sunlight.h"
#include"plantarea.h"
#include"nomalzombie.h"
#include"screenzombie.h"
#include"conezombie.h"
#include"footballzombie.h"
#include"bucketzombie.h"
#include"mower.h"
#include"selectplant.h"
#include"startwidget.h"
#include"settingsmenu.h"
#include"shovel.h"
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QSoundEffect>
#include"myobj.h"
#include"plant.h"
#include"zombie.h"
#include<QSettings>
#include"settingsmenu.h"

class Dominator;

class GameScene : public QGraphicsScene
{
    Q_OBJECT
    //dominator
    Dominator* dominator;
    //外部控件，不跟随gamescene生命周期
    SettingsMenu *settingsMenu;

    //archive
    QSettings *settings;
    //相关控件
    Shop *shop;//商店
    SelectPlant *selectPlant;//选择板
    Shovel * shovel;//铲子
    //各类物品集合
    QList<PlantArea*> plantareas;
    QList<Plant*> plants;
    QList<Zombie*> zombies;
    //依靠行将控件分组
    QMap<int,QList<Plant*> > plantRow;
    QMap<int,QList<Zombie*> > zombieRow;
    QMap<int,QList<PlantArea*>> plantAreaRow;

    //音效
    QMediaPlayer *bgMus;//背景音播放器
    QAudioOutput *audioOutput;

    //背景
    QString bgPath;
    QGraphicsPixmapItem *gameBg;
    //QTimer
    QTimer *waveTimer;

    void moveBg();

public:
    explicit GameScene(QObject *parent = nullptr);
    void PlantAreaGenerate();
    void ZombieGenerate();
    void cardAvailable();
    void menuInit();
    void settingInit();
    void GameStart();
    void GamePre();
    void playBGM();
    //函数
    void addItem(MyObject* item);
    void addItem(QGraphicsItem* item);

    void setMenu(SettingsMenu *settingsMenu){settingsMenu = settingsMenu;}

    void plant(enum PlantType plantType);//种植植物
    void move(MyObject* target,QPointF& dest);//移动物体
    void DominatorAct();//处理dominator行动逻辑
signals:
    void GameOver();
    void GamePause();
    void GameContinue();

    
};

#endif // GAMESCENE_H
