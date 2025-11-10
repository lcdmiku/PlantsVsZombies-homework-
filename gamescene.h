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


class GameScene : public QGraphicsScene
{
    Q_OBJECT
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

    //QTimer
    QTimer *waveTimer;

    void PlantAreaGenerate();
    void ZombieGenerate();
    void cardAvailable();
    void GameStart();
    void GamePre();
public:
    explicit GameScene(QObject *parent = nullptr);

public slots:
    void plant();//种植植物
    void move(MyObject* target,QPointF& dest);//移动物体

    
};

#endif // GAMESCENE_H
