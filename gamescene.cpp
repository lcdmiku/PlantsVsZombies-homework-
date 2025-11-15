#include "gamescene.h"
#include"animate.h"
#include<QAudioOutput>
#include<QGraphicsProxyWidget>
#include<QTimeLine>
#include<QGraphicsItemAnimation>
#include"dominator.h"

GameScene::GameScene(QObject *parent,GameLevelData* data)
    : QGraphicsScene(parent),settingsMenu(nullptr),levelData(data),
    plantareas(),zombies(),plants(),
    plantAreaMap(5,QList<PlantArea*>(9,nullptr)),plantRow(),zombieRow(),
    bgPath(":/res/GameRes/images/Background.jpg"),gameBg(nullptr),
    dominator(nullptr)
{
    //archive
    settings = new QSettings("config.ini",QSettings::IniFormat);
    settingInit();
    //相关控件
    shop = new Shop();//商店
    addItem(shop);
    selectPlant = new SelectPlant();//选择板
    addItem(selectPlant);
    shovel = new Shovel;//铲子
    addItem(shovel);
    gameBg = new QGraphicsPixmapItem(QPixmap(bgPath));
    addItem(gameBg);
    dominator = new Dominator();
    addItem(dominator);

    // coo = new Coordinate(0);


    //QTimer
    waveTimer = new QTimer(this);
    waveTimer->start(1000);
    waveTimer->stop();
}
void GameScene::settingInit(){
    // 如果配置中没有 MapInfo 节，写入默认配置（以 QVariantList 格式写入，便于后续 .toList() 读取）
    settings->beginGroup("MapInfo");
    bool needInitMap = settings->childKeys().isEmpty();
    settings->endGroup();
    if(needInitMap){
        qDebug() << "config.ini MapInfo missing — writing default MapInfo.";
        QVariantList defaultRow;
        for(int k=0;k<9;k++) defaultRow << 0;
        settings->beginGroup("MapInfo");
        for(int i=0;i<5;i++){
            settings->setValue(QString("row%1").arg(i), defaultRow);
        }
        QVariantList mowerRow;
        for(int i=0;i<5;i++) mowerRow << 1;
        settings->setValue("MowerRow", mowerRow);
        QVariantList zombieRow;
        for(int i=0;i<5;i++) zombieRow << i;
        settings->setValue("zombieRow", zombieRow);
        settings->setValue("MowerPower", 99999);
        settings->endGroup();
        settings->sync();
    }
}
void GameScene::menuInit(){
    connect(settingsMenu,&SettingsMenu::GamePause,this,&GameScene::GamePause);
    connect(settingsMenu,&SettingsMenu::GameContinue,this,&GameScene::GameContinue);
    connect(settingsMenu,&SettingsMenu::getBack,this,[=](){
            emit GameOver();//退出就结束游戏
    });
}

void GameScene::playBGM(){
    //背景音乐
    bgMus = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(0.5);
    connect(settingsMenu,&SettingsMenu::volumeChanged,this,[=](int volume){
        audioOutput->setVolume(double(volume)/100);
    });
    bgMus->setAudioOutput(audioOutput);
    bgMus->setSource(QUrl("qrc:/res/GameRes/GrazyDave2.mp3"));
    bgMus->setLoops(-1);
    bgMus->play();
}
//处理dominator行动逻辑
void GameScene::DominatorAct(){
    if(!dominator)return;
    dominator->setPos(this->sceneRect().center());
    dominator->setZValue(10);
}
//选这植物阶段
void GameScene::GamePre(){

    //选择版
    cardAvailable();
    selectPlant->setPos(290,100);
    //设置背景
    gameBg->setPos(-330,0);
    qDebug()<<gameBg->pos()<<gameBg->x()<<gameBg->y();
    gameBg->setZValue(-100);

    //设置开始按键
    QPushButton *startBtn = new QPushButton("Start");
    QPalette palette_Btn;//按键统一样式
    palette_Btn.setBrush(QPalette::Button,QBrush(QPixmap(":/res/GameRes/images/Button.png")));
    startBtn->setPalette(palette_Btn);
    startBtn->resize(100,40);
    QGraphicsProxyWidget *start_proxy = addWidget(startBtn);
    //设置按钮在场景中的位置
    start_proxy->setPos(900,500);
    //设置卡片清空按键
    QPushButton *cardDeleteBtn = new QPushButton("Clear Cards");
    cardDeleteBtn->setPalette(palette_Btn);
    cardDeleteBtn->resize(100,40);
    QGraphicsProxyWidget *cardDelete_proxy = addWidget(cardDeleteBtn);
    //设置按钮在场景中的位置
    cardDelete_proxy->setPos(900,400);

    //选择结束
    connect(startBtn,&QPushButton::clicked,this,[=](){
        emit GameContinue();
        start_proxy->hide();
        cardDelete_proxy->hide();
        waveTimer->start();
        moveBg();
        Animate(selectPlant).duration(AnimationType::Move,1000).move(QPointF(0,-600));

        QTimer::singleShot(1200,this,[=](){
            GameStart();
        });

    });
    connect(this,&GameScene::GameOver,this,[=](){
        start_proxy->show();
        cardDelete_proxy->show();
    });


    setItemIndexMethod(QGraphicsScene::NoIndex);


    //选择卡片逻辑
    connect(selectPlant,&SelectPlant::cardPress,shop,[=](Card *card){
        shop ->addCard(card->getPlantPath());
    });
    connect(cardDeleteBtn,&QPushButton::clicked,this,[=](){
        if(shop)shop->clearCards();
        selectPlant->reSet();
    });
    connect(this,&GameScene::GameOver,shop,[=](){
        if(shop)shop->clearCards();
        selectPlant->reSet();
    });
    shop->setPos(290, 0);//商店位置
}
void GameScene::GameStart(){
    //dominator
    DominatorAct();
    //shovel
    shovel->setPos(shovel->getStartPos());

    //sunlight generate
    connect(waveTimer,&QTimer::timeout,this,[=](){
        int gen = QRandomGenerator::global()->bounded(1,20);
        int x = QRandomGenerator::global()->generateDouble()*this->width();
        int y = QRandomGenerator::global()->generateDouble()*this->height();
        if(gen == 1){
            SunLight *sunlight = new SunLight;
            sunlight->setPos(x,y);
            //仅仅收集阳光使用bool(int)
            bool (Shop::*func)(int) = &Shop::sunlightValueShow;
            connect(sunlight,&SunLight::sunlightCollected,shop,func);

            addItem(sunlight);
            connect(this,&GameScene::GameOver,sunlight,&QGraphicsObject::deleteLater);
        }
    });

    //zombie generate
    QMetaObject::Connection Conn;
    Conn = connect(waveTimer,&QTimer::timeout,this,[=](){
        int gen = QRandomGenerator::global()->bounded(0,1);//僵尸数量越大，越不用以生成
        if(gen == 0)
        {
            ZombieGenerate();
        }
    });

    //add card
    //cardADD();

    //plantarea
    PlantAreaGenerate();

    //zombie generate
    ZombieGenerate();
}
void GameScene::move(MyObject* target,QPointF& dest){
        Animate(target).move(dest,false);
}
//种植植物
void GameScene::plant(enum PlantType plantType,int r,int c){
    PlantArea * area = getPlantArea(r,c);
    if(area){
        area->plant(plantType);
    }
    else{
        qDebug()<<"failde to plant";
    }
}
//获得种植地
PlantArea* GameScene::getPlantArea(int r,int c){
    if(r>=0 && c>=0 && r<plantAreaMap.size() && c<plantAreaMap[0].size()){
        return plantAreaMap[r][c];
    }
    return nullptr;
}
void GameScene::moveBg(){
    int duration = 1000;
    int hz = 10;
    QTimeLine* timeLine = new QTimeLine(duration); // 动画时长1000ms

    timeLine->setUpdateInterval(hz);
    timeLine->setFrameRange(0, duration/hz); // 帧范围
    timeLine->setEasingCurve(QEasingCurve::Linear);
    timeLine->setLoopCount(1);

    QGraphicsItemAnimation* moveAnim = new QGraphicsItemAnimation();
    moveAnim->setItem(gameBg);
    moveAnim->setTimeLine(timeLine);

    moveAnim->setPosAt(0, gameBg->pos());
    moveAnim->setPosAt(1, QPointF(0,0));

    connect(timeLine, &QTimeLine::finished,this, [=]() {
        timeLine->deleteLater();
        moveAnim->deleteLater();
    });
    timeLine->start();
}
void GameScene::PlantAreaGenerate(){
    //打开配置文件
    settings->beginGroup("MapInfo");
    //
    for(int i=0;i<5;i++){
        QVariantList MapRow = settings->value(QString("row%1").arg(i)).toList();
        QVariantList MowerRow = settings->value(QString("MowerRow")).toList();
        //Mower
        if(MowerRow[i].toInt()){
            Mower *mower = new Mower();
            mower->setPos(QPointF(150 +105 ,120) + QPointF(-20 ,94*i));
            addItem(mower);
            connect(this,&GameScene::GameOver,mower,&MyObject::GameOver);
        }
        for(int j=0;j<9;j++){
            enum LandType landType = LandType::None;
            switch (MapRow[j].toInt()) {
            case 0:{
                landType = LandType::Nomal;
                break;
            }
            case 1:{
                landType = LandType::DryLand;
                break;
            }
            default:
                break;
            }
            PlantArea *area = new PlantArea(i,j,landType);
            //将实例加入集合
            plantareas.push_back(area);
            plantAreaMap[i][j] = area;
            //设置位置
            area->setPos(QPointF(150 +105 ,90) + QPointF(area->w()*j , area->h()*i));//81,94
            //连接向日葵生成的阳光
            connect(area,&PlantArea::sunlightProduced,this,[=](SunLight *sunlight){
                //仅仅收集阳光使用bool(int)
                bool (Shop::*func)(int) = &Shop::sunlightValueShow;
                connect(sunlight,&SunLight::sunlightCollected,shop,func);
                connect(this,&GameScene::GameOver,sunlight,&QGraphicsObject::deleteLater);
            });
            //买并种植植物消耗阳光
            connect(area,&PlantArea::needToPlant,this,[=](int cost,enum PlantType plantType){
                //如果阳光足够
                if(shop->sunlightValueShow(-cost,plantType)){
                    area->plant(plantType);
                }
            });
            addItem(area);
            connect(this,&GameScene::GameOver,area,&PlantArea::GameOver);

        }
    }
    //
    settings->endGroup();
}

void GameScene::ZombieGenerate(){
    //打开配置文件
    settings->beginGroup("MapInfo");
    QVariantList zombieRow = settings->value("zombieRow").toList();
    settings->endGroup();
    //
    int index = QRandomGenerator::global()->bounded(0,zombieRow.size());//随机1到5行
    int row = zombieRow[index].toInt();
    //
    double y = 100 + row*94;
    int offsetX = QRandomGenerator::global()->bounded(0,100);//避免僵尸同时出现，用距离控制时间
    QPointF start(this->width()+200+offsetX,y);
    // QPointF end(100,y);
    //
    int gen = QRandomGenerator::global()->bounded(0,11);//随机0到4
    Zombie *zombie=nullptr;
    //随机决定僵尸种类
    if(gen<3)
        zombie = new NomalZombie();
    else if(gen<5){
        zombie = new ScreenZombie();
    }
    else if(gen<7){
        zombie = new BucketZombie();
    }
    else if(gen<9){
        zombie = new ConeZombie();
    }
    else if(gen<11){
        zombie = new FootballZombie();
    }
    if(zombie)
    {
        zombie->setPos(start);
        zombies.push_back(zombie);
        this->zombieRow[row].push_back(zombie);
        zombie->setZValue(row);
        addItem(zombie);
        //僵尸行走
        Animate(zombie).speed(AnimationType::Move,zombie->getSpeed()).move(QPointF(-900,0));
        //处理僵尸胜利的请款
        connect(zombie,&Zombie::zombieSuccess,this,[=](){

        });
        connect(this,&GameScene::GameOver,zombie,&MyObject::GameOver);
    }
}

void GameScene::ZombieGenerate(ZombieType zombieType,int row,int x){
    Zombie *zombie=nullptr;
    switch (zombieType)
    {
    case ZombieType::NormalZombie:
    {
        zombie = new NomalZombie();
        break;
    }
    case ZombieType::ScreenZombie:
    {
        zombie = new ScreenZombie();
        break;
    }

    case ZombieType::BucketZombie:
    {
        zombie = new BucketZombie();
        break;
    }
    case ZombieType::ConeZombie:
    {
        zombie = new ConeZombie();
        break;
    }
    case ZombieType::FootballZombie:
    {
        zombie = new FootballZombie();
        break;
    }
    default:
        break;
    }
    if(zombie)
    {
        zombie->setPos(QPointF(0 ,120) + QPointF(x ,94*row));
        zombies.push_back(zombie);
        this->zombieRow[row].push_back(zombie);
        zombie->setZValue(row);
        addItem(zombie);
        //僵尸行走
        Animate(zombie).speed(AnimationType::Move,zombie->getSpeed()).move(QPointF(-900,0));
        //处理僵尸胜利的情况
        connect(zombie,&Zombie::zombieSuccess,this,[=](){

        });
        connect(this,&GameScene::GameOver,zombie,&MyObject::GameOver);//与消亡绑定
    }
}

void GameScene::cardAvailable(){
    if(selectPlant)
    {
        selectPlant->addCard(":/res/GameRes/images/WallNut.png");
        selectPlant->addCard(":/res/GameRes/images/Peashooter.png");
        selectPlant->addCard(":/res/GameRes/images/SunFlower.png");
        selectPlant->addCard(":/res/GameRes/images/CherryBomb.png");
        selectPlant->addCard(":/res/GameRes/images/PotatoMine.png");
        selectPlant->addCard(":/res/GameRes/images/SnowPea.png");
    }
}

void GameScene::addItem(QGraphicsItem* item){
    QGraphicsScene::addItem(item);
}
void GameScene::addItem(MyObject* item){
    QGraphicsScene::addItem(item);

    connect(this,&GameScene::GamePause,item,&MyObject::GamePause);
    connect(this,&GameScene::GameContinue,item,&MyObject::GameContinue);
    connect(this,&GameScene::GameOver,item,&MyObject::GameOver);
}

//调试用
// void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event){
//     if(event->button() == Qt::LeftButton){
//         qDebug() << event->scenePos().x() << event->scenePos().y();
//         qreal x = event->scenePos().x();
//         qreal y = event->scenePos().y();
//         qDebug() << "col" << coo->getCol(x);
//         qDebug() << "row" << coo->getRow(y);
//         qDebug() << "centerX" << coo->getX(coo->getCol(x));
//         qDebug() << "centerY" << coo->getY(coo->getRow(y));
//     }
//     QGraphicsScene::mousePressEvent(event);
// }
