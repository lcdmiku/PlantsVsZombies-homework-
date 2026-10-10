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
#include"trophy.h"
#include<QSettings>
#include"settingsmenu.h"
#include"gameleveldata.h"
#include "flagmeter.h"

#include<QGraphicsSceneMouseEvent>
#include "coordinate.h"
#include"soundmanager.h"

class Dominator;

class GameScene : public QGraphicsScene
{
    Q_OBJECT
    //dominator
    Dominator* dominator;
    //外部控件，不跟随gamescene生命周期
    SettingsMenu *settingsMenu;
    //

    FlagMeter* flagMeter;

    //archive
    QSettings *settings;
    //相关控件
    Shop *shop;//商店
    SelectPlant *selectPlant;//选择板
    Shovel * shovel;//铲子
    //各类物品集合
    QList<PlantArea*> plantareas;
    QMap<int,Mower*> mowers;
    //依靠行将控件分组
    QList<QList<PlantArea*>> plantAreaMap;//位置存储各个种植地

    //音效
    QMediaPlayer *bgMus;//背景音播放器
    QAudioOutput *audioOutput;
    SoundManager* soundManager;

    //背景
    QString bgPath;
    QGraphicsPixmapItem *gameBg;
    //QTimer 控制游戏时间
    QTimer *waveTimer;
    int currWave;//记录已经是第几波

    //wave
    int waveMoment;//记录当前波已经进行的时间
    int moment;//记录游戏已经进行的时间

    void moveBg();/*
    Coordinate* coo;*/

    // 代理控件
    QGraphicsProxyWidget *start_proxy;
    QGraphicsProxyWidget *cardDelete_proxy;

public:
    explicit GameScene(QObject *parent = nullptr,GameLevelData* data = new GameLevelData_1());
    ~GameScene();
    void PlantAreaGenerate();

    GameLevelData* levelData;

    void cardAvailable();
    void menuInit();
    void settingInit();
    void GameStart();
    void GamePre();
    void playBGM(const QString& BgmPath);
    //函数
    void addItem(MyObject* item);
    void addItem(QGraphicsItem* item);

    void setMenu(SettingsMenu *menu){
        this->settingsMenu = menu;
        menuInit();
    }

    //得到种植地
    PlantArea* getPlantArea(int r,int c);
    //得到僵尸集合
    QList<Zombie*> getZombies();
    QList<Zombie*> getZombiesRow(int r);
    //得到商店
    Shop* getShop(){return shop;}
    //得到铲子
    Shovel* getShovel(){return shovel;}
    //得到dominator
    Dominator* getDominator(){return dominator;}
    //得到游戏已经进行的时间
    //得到小推车的Map引用
    QMap<int,Mower*>& getMowers(){return mowers;}
    int getMoment(){return moment;}
    //得到当前wave
    int getCurrWave(){return currWave;}

    //植物与僵尸的生成
    void ZombieGenerate();
    void ZombieGenerate(ZombieType zombieType,int row,int x);
    void ZombieGenerate(int currwave);//根据当前波数生成僵尸
    void plant(enum PlantType plantType,int r,int c);//种植植物
    void move(MyObject* target,QPointF& dest);//移动物体
    void DominatorAct();//处理dominator行动逻辑
    //处理阳光生成
    void sunlightGenerate(int prob=20);//每秒生成阳光的可能性 prob%,0-100
    //处理小推车生成
    void mowerGenerate();
    void mowerGenerate(int r);
    void releaseMower(int r);

    // void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    //处理进入下一波
    void setNextWave();
    //处理僵尸胜利
    void showZombieWon();
    //处理玩家胜利
    void showPlayerWon(QPointF pos = QPointF(600, 300));
    void showPlayerWon();
    //处理音频缓存
    //播放短时音效
    void playSoundEffect(const QString& soundPath);

    //场景元素过滤
    template<typename T>
    QList<T*> filterGameScene(std::function<bool(T*)> functor){
        QList<QGraphicsItem*> allItems = this->items();
        QList<QGraphicsItem*> filtered;
        std::copy_if(allItems.begin(), allItems.end(), std::back_inserter(filtered),
                         [functor](QGraphicsItem *item) {
                            T *customItem = dynamic_cast<T*>(item);
                            return customItem && customItem->isActive() && functor(customItem);
                        });
        QList<T*> res;
        foreach (QGraphicsItem* ptr, filtered) {
            T* obj = dynamic_cast<T*>(ptr);
            if(obj)res.push_back(obj);
        }
        return res;

    }


signals:
    void GameOver();
    void GamePause();
    void GameContinue();
    void GameSuccess(bool isOk);//胜利或者不胜利都记录在内
    void waveStart(int currWave);
    void nextWave();//强制性下一波

    
};



#endif // GAMESCENE_H
