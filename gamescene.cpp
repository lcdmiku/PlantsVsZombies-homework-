#include "gamescene.h"
#include"animate.h"
#include<QAudioOutput>
#include<QGraphicsProxyWidget>
#include<QTimeLine>
#include<QGraphicsItemAnimation>
#include"dominator.h"

GameScene::GameScene(QObject *parent,GameLevelData* data)
    : QGraphicsScene(parent),settingsMenu(nullptr),levelData(data),moment(0),
    plantareas(),zombies(),plants(),
    plantAreaMap(5,QList<PlantArea*>(9,nullptr)),plantRow(),zombieRow(),
    bgPath(data->backgroundImage),gameBg(nullptr),
    bgMus(new QMediaPlayer(this)),audioOutput(new QAudioOutput(this)),
    dominator(nullptr)
{
    //检查gameleveldata是否为空,若为空，输出问题并退出
    if(!levelData){
        qDebug()<<"gameleveldata is nullptr,failed to create gamescene";
        deleteLater();
    }
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
    //音效
    audioOutput->setVolume(0.5);
    bgMus->setAudioOutput(audioOutput);
    bgMus->setLoops(-1);
    connect(settingsMenu,&SettingsMenu::volumeChanged,this,[=](int volume){
        audioOutput->setVolume(double(volume)/100);
    });
    // coo = new Coordinate(0);


    //QTimer
    waveTimer = new QTimer(this);
    waveTimer->start(1000);
    waveTimer->stop();
    connect(waveTimer,&QTimer::timeout,this,[=](){
        moment++;//记录游戏已经进行的时间
    });
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
//背景音乐
void GameScene::playBGM(const QString& BgmPath){
    bgMus->blockSignals(true);
    bgMus->stop();
    bgMus->blockSignals(false);
    bgMus->setSource(QUrl(BgmPath));//设置背景音
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
//处理阳光生成
void GameScene::sunlightGenerate(int prob){
    connect(waveTimer,&QTimer::timeout,this,[=](){
        int gen = QRandomGenerator::global()->bounded(1,100);
        int x = QRandomGenerator::global()->generateDouble()*this->width();
        int y = QRandomGenerator::global()->generateDouble()*this->height();
        if(gen <= prob){
            SunLight *sunlight = new SunLight;
            sunlight->setPos(x,y);
            //仅仅收集阳光使用bool(int)
            bool (Shop::*func)(int) = &Shop::sunlightValueShow;
            connect(sunlight,&SunLight::sunlightCollected,shop,func);

            addItem(sunlight);
            connect(this,&GameScene::GameOver,sunlight,&QGraphicsObject::deleteLater);
        }
    });
}
//进行游戏阶段
void GameScene::GameStart(){
    //背景音
    playBGM(levelData->backgroundMusic);
    //dominator
    DominatorAct();
    //shovel
    shovel->setPos(shovel->getStartPos());

    //sunlight generate
    sunlightGenerate();

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
    //mower
    mowerGenerate();

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
//移动背景动画
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
//生成小推车
void GameScene::mowerGenerate(){
    //
    for(int i=0;i<5;i++){
        QList MowerRow = levelData->mowerRow;
        //Mower
        if(MowerRow[i]==1){
            Mower *mower = new Mower();
            mower->setPos(QPointF(150 +105 ,120) + QPointF(-20 ,94*i));
            addItem(mower);
            connect(this,&GameScene::GameOver,mower,&MyObject::GameOver);
        }
    }

}
//生成种植地
void GameScene::PlantAreaGenerate(){
    //打开配置文件
    settings->beginGroup("MapInfo");
    //
    for(int i=0;i<5;i++){
        QVariantList MapRow = settings->value(QString("row%1").arg(i)).toList();
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
//生成僵尸
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
//给selectpanel加载卡片
void GameScene::cardAvailable(){
    if(selectPlant)
    {
        for (int var = 0; var < levelData->pName.size(); ++var) {
            QString path = ":/res/GameRes/images/" + levelData->pName[var] + ".png";
            selectPlant->addCard(path);
        }
    }
}
//添加元素到gamescene
void GameScene::addItem(QGraphicsItem* item){
    QGraphicsScene::addItem(item);
}
void GameScene::addItem(MyObject* item){
    QGraphicsScene::addItem(item);

    connect(this,&GameScene::GamePause,item,&MyObject::GamePause);
    connect(this,&GameScene::GameContinue,item,&MyObject::GameContinue);
    connect(this,&GameScene::GameOver,item,&MyObject::GameOver);
}

//播放短时音效
void playSoundEffect(const QString& soundPath){
    QMediaPlayer* player = new QMediaPlayer;
    QAudioOutput* audioOutput = new QAudioOutput;

    audioOutput->setVolume(0.5);

    player->setAudioOutput(audioOutput);
    player->setSource(soundPath);
    player->play();
    //内存管理
    QMediaPlayer::connect(player, &QMediaPlayer::playbackStateChanged, player, [=](QMediaPlayer::PlaybackState state) {
        if (state == QMediaPlayer::StoppedState) {
            player->deleteLater();
            audioOutput->deleteLater();
        }
    });
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
