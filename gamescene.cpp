#include "gamescene.h"
#include"animate.h"
#include<QAudioOutput>
#include<QGraphicsProxyWidget>
#include<QTimeLine>
#include<QGraphicsItemAnimation>
#include<QPropertyAnimation>
#include"dominator.h"
#include<QGraphicsView>
#include<QDebug>
#include "zomboni.h"


GameScene::GameScene(QObject *parent,GameLevelData* data)
    : QGraphicsScene(parent),settingsMenu(nullptr),levelData(data),
    moment(0),currWave(0),waveMoment(0),
    plantareas(),mowers(),
    plantAreaMap(5,QList<PlantArea*>(9,nullptr)),
    bgPath(data->backgroundImage),gameBg(nullptr),
    bgMus(new QMediaPlayer(this)),audioOutput(new QAudioOutput(this)),
    dominator(nullptr), flagMeter(nullptr),
    soundManager(new SoundManager(this))
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

    shovel = new Shovel;//铲子,在~GameScene中delete
    addItem(shovel);

    gameBg = new QGraphicsPixmapItem(QPixmap(bgPath));//背景，在~GameScene中delete
    addItem(gameBg);

    //改为datalevel存储dominator实例类
    dominator = levelData->dominator;
    addItem(dominator);

    // 进度条
    flagMeter = new FlagMeter(levelData);
    addItem(flagMeter);

    //位置 750，300
    flagMeter->setPos(860, 570);

    //音效
    audioOutput->setVolume(0.5);
    bgMus->setAudioOutput(audioOutput);
    bgMus->setLoops(-1);
    connect(settingsMenu,&SettingsMenu::volumeChanged,this,[=](int volume){
        audioOutput->setVolume(qreal(volume)/100.0);
    });
    // coo = new Coordinate(0);

    start_proxy = nullptr;
    cardDelete_proxy = nullptr;


    //QTimer
    waveTimer = new QTimer(this);
    waveTimer->start(1000);
    waveTimer->stop();
    //强制性下一波
    connect(this,&GameScene::nextWave,this,[=](){
        setNextWave();
    });
    connect(waveTimer,&QTimer::timeout,this,[=](){
        moment++;//记录游戏已经进行的时间
        waveMoment++;
        if(flagMeter) flagMeter->updateProgress(moment);
        //当前波结束，开始下一波计时
        if(currWave < levelData->waveNum && waveMoment>=levelData->waveDuration[currWave]){
            setNextWave();
        }

    });
    //处理胜利与失败
    // connect(this,&GameScene::GameSuccess,this,[=](bool isOk){
    //     if(isOk){

    //     }
    // });
}
//处理胜利

//处理进入下一波
void GameScene::setNextWave(){
    waveMoment = 0;
    currWave++;
    emit waveStart(currWave);//发送当前波开始的信号
    qDebug()<<"now the"<<currWave<<"-th wave";
    //当全部结束，标志胜利
    if(currWave>=levelData->waveNum){
        emit GameSuccess(true);
        // showPlayerWon(); // 改为在最后一个僵尸死亡时触发
    }
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
    settings->beginGroup("LevelInfo");
    if(settings->childKeys().isEmpty()){
        settings->setValue("unlockedLevel",1);//解锁关卡数
        settings->setValue("limitLevel",5);//最多关卡数
    }
    settings->endGroup();
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

    connect(this,&GameScene::waveStart,dominator,&Dominator::waveStart);//使dominator能感应外界波次
    dominator->initEvent();
}
//选择植物阶段
void GameScene::GamePre(){

    //选择版
    cardAvailable();
    if(selectPlant) selectPlant->setPos(290,100);


    //设置背景
    if(gameBg) {
        gameBg->setPos(-330,0);
        qDebug()<<gameBg->pos()<<gameBg->x()<<gameBg->y();
        gameBg->setZValue(-100);
    }


    //设置开始按键
    if(!start_proxy) {
        QPushButton *startBtn = new QPushButton("Start");
        QPalette palette_Btn;//按键统一样式
        palette_Btn.setBrush(QPalette::Button,QBrush(QPixmap(":/res/GameRes/images/Button.png")));
        startBtn->setPalette(palette_Btn);
        startBtn->resize(100,40);
        start_proxy = addWidget(startBtn);
        //设置按钮在场景中的位置
        start_proxy->setPos(900,500);

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
    } else {
        start_proxy->show();
    }

    //设置卡片清空按键
    if(!cardDelete_proxy) {
        QPushButton *cardDeleteBtn = new QPushButton("Clear Cards");
        QPalette palette_Btn;
        palette_Btn.setBrush(QPalette::Button,QBrush(QPixmap(":/res/GameRes/images/Button.png")));
        cardDeleteBtn->setPalette(palette_Btn);
        cardDeleteBtn->resize(100,40);
        cardDelete_proxy = addWidget(cardDeleteBtn);
        //设置按钮在场景中的位置
        cardDelete_proxy->setPos(900,400);

        connect(cardDeleteBtn,&QPushButton::clicked,this,[=](){
            if(shop)shop->clearCards();
            selectPlant->reSet();
        });
    } else {
        cardDelete_proxy->show();
    }

    setItemIndexMethod(QGraphicsScene::NoIndex);


    //选择卡片逻辑
    connect(selectPlant,&SelectPlant::cardPress,shop,[=](Card *card){
        shop ->addCard(card->getPlantPath());
    });

    shop->setPos(290, 0);//商店位置
    qDebug()<<"No problem now!";
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
    playBGM(levelData->backgroundMusic);//太吵了,先注释掉
    //dominator
    DominatorAct();
    //shovel
    shovel->setPos(shovel->getStartPos());
    if(!levelData->hasShovel)shovel->hide();//如果关卡没有铲子，将铲子隐藏

    //sunlight generate
    sunlightGenerate(levelData->sunProb);

    //每一波开始后
    connect(this,&GameScene::waveStart,this,[=](int currwave){
        ZombieGenerate(currwave);
        levelData->dominatorAct(this);
    });

    //plantarea
    PlantAreaGenerate();
    //mower
    mowerGenerate();//暂时注释，调试完放回

    //zombie generate
    // ZombieGenerate();
    //第0波开始
    emit waveStart(0);
    if(flagMeter) flagMeter->showMeter();

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
    //暂时注释，调试完放回去
    for(int i=0;i<5;i++){
        QList MowerRow = levelData->mowerRow;
        //Mower
        if(MowerRow[i]==1){
            mowerGenerate(i);
        }
    }

}
void GameScene::mowerGenerate(int r){
    if(!mowers.contains(r))
    {
        Mower *mower = new Mower();
        mowers.insert(r,mower);//加入mower
        mower->setPos(QPointF(150 +105 ,120) + QPointF(-20 ,94*r));
        mower->setZValue(-1);
        addItem(mower);
        connect(this,&GameScene::GameOver,mower,&MyObject::GameOver);
    }
}
//移除小推车
void GameScene::releaseMower(int r){
    if(mowers.contains(r))
    {
        Mower *mower = mowers[r];
        if(mower){
            mower->releaseMower();//使mower开动
        }
        mowers.remove(r);//移除map
    }
}
//生成种植地
void GameScene::PlantAreaGenerate(){
    for(int i=0;i<5;i++){
        QList MapRow = levelData->map[i];
        for(int j=0;j<9;j++){
            enum LandType landType = LandType::None;
            switch (MapRow[j]) {
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
            plantAreaMap[i][j] = area;//TODO
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
    double y = 100 + row*94;//
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
        zombie->setZValue(row);
        addItem(zombie);
        //僵尸行走
        Animate(zombie).speed(AnimationType::Move,zombie->getSpeed()).move(QPointF(-1800,0));
        //处理僵尸胜利的情况：当僵尸触发胜利时，标记为关卡失败并结束游戏
        connect(zombie,&Zombie::zombieSuccess,this,&GameScene::showZombieWon);
        connect(this,&GameScene::GameOver,zombie,&MyObject::GameOver);
        connect(this,&GameScene::GamePause,zombie,&MyObject::GamePause);
        
        // 连接僵尸全部死亡信号
        connect(zombie, &Zombie::noZombie, this, [=](QPointF pos){
            // 只要是最后一波（或之后），且僵尸全部死亡，就胜利
            if(currWave >= levelData->waveNum - 1){
                showPlayerWon(pos);
            }
        });

        // 检查护眼模式
        if(dominator && dominator->isEyeProtectionOn()){
            zombie->setSlow(true);
        }
    }
}
//在指定位置生成僵尸
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
    case ZombieType::Zomboni:
    {
        Zomboni* zomboni = new Zomboni();
        // 应用关卡难度设置
        if (levelData) {
            zomboni->setMySummonInterval(levelData->zomboniSummonInterval);
            zomboni->setMySelfSummonProbability(levelData->zomboniSelfSummonProb);
            zomboni->reduceHP(levelData->zomboniHPRate);
            zomboni->reduceSpeed(levelData->zomboniSpeedRate);
        }
        zombie = zomboni;
        break;
    }
    default:
        break;
    }
    if(zombie)
    {
        zombie->setPos(QPointF(0 ,100) + QPointF(x ,94*row));

        zombie->setZValue(row);
        addItem(zombie);
        //僵尸行走
        Animate(zombie).speed(AnimationType::Move,zombie->getSpeed()).move(QPointF(-1800,0));
        //处理僵尸胜利的情况：当僵尸触发胜利时，标记为关卡失败并结束游戏
        connect(zombie,&Zombie::zombieSuccess,this,&GameScene::showZombieWon);
        connect(this,&GameScene::GameOver,zombie,&MyObject::GameOver);//与消亡绑定
        connect(this,&GameScene::GamePause,zombie,&MyObject::GamePause);

        // 连接僵尸全部死亡信号
        connect(zombie, &Zombie::noZombie, this, [=](QPointF pos){
            // 只要是最后一波（或之后），且僵尸全部死亡，就胜利
            if(currWave >= levelData->waveNum - 1){
                showPlayerWon(pos);
            }
        });

        // 检查护眼模式
        if(dominator && dominator->isEyeProtectionOn()){
            zombie->setSlow(true);
        }
    }
}
//根据当前波数生成僵尸
void GameScene::ZombieGenerate(int currwave){

    if(currwave == 0)return;
    // 第6波只生成一个冰车僵尸
    if(currwave == 6) {
        qDebug() << "yes, I'm generating zomboni!";
        int row = QRandomGenerator::global()->bounded(0,5);
        ZombieGenerate(ZombieType::Zomboni, row, this->width()+200);
        return; // 直接返回，不生成其他僵尸
    }


    QList<ZombieType> zombies = levelData->zombieExtract(currwave);
    for (int i = 0; i < zombies.size(); ++i)
    {
        int row = QRandomGenerator::global()->bounded(0,5);//随机投发到row行
        //随机出现时间/s,每波开始后至少6s后再刷新僵尸
        int showTime = QRandomGenerator::global()->bounded(6,levelData->waveDuration[currwave]);
        //将僵尸生存时间分开
        QTimer::singleShot(showTime*1000,this,[=](){
            ZombieGenerate(zombies[i],row,this->width()+130);
        }) ;
    }
}
//给selectpanel加载卡片
void GameScene::cardAvailable(){
    qDebug() << "into cardAvailable()函数";
    if(selectPlant && levelData)
    {
        // 先清空，防止重复添加，clearCards是删除所有卡片，免得重复创造
        selectPlant->clearCards();
        for (int var = 0; var < levelData->pName.size(); ++var) {
            QString path = ":/res/GameRes/images/" + levelData->pName[var] + ".png";
            selectPlant->addCard(path);
        }
    }
    qDebug() << "out of cardAvailable()函数";
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
//析构函数
GameScene::~GameScene(){
    delete soundManager;
    disconnect();
    // delete shovel; // 场景会自动删除这些 item
    // delete gameBg;
}


void GameScene::showZombieWon(){
    // 停止波次计时
    if(waveTimer->isActive()) waveTimer->stop();

    // 停止背景音乐
    bgMus->stop();

    // 播放音效
    playSoundEffect("qrc:/res/GameRes/audio/scream.wav");
    playSoundEffect("qrc:/res/GameRes/audio/losemusic.mp3");

    // 定住所有僵尸和植物
    emit GamePause();

    // 获取视图
    QList<QGraphicsView *> views = this->views();
    if (views.isEmpty()) return;
    QGraphicsView* view = views.first();

    // 视口平移动画
    QRectF startRect = view->sceneRect();
    // 向左移动视口 250 像素（即看到左边的内容），相当于背景向右移
    QRectF endRect = startRect.translated(-150, 0);

    QTimeLine* timeLine = new QTimeLine(2000, this);
    timeLine->setFrameRange(0, 100);
    timeLine->setUpdateInterval(20);
    timeLine->setEasingCurve(QEasingCurve::InOutQuad);

    // 显示图片
    QGraphicsPixmapItem* wonItem = new QGraphicsPixmapItem(QPixmap(":/res/GameRes/images/ZombiesWon.png"));
    wonItem->setZValue(100);
    addItem(wonItem);

    // 动画更新
    connect(timeLine, &QTimeLine::valueChanged, this, [=](qreal value){
        // 插值计算当前 sceneRect
        qreal t = value;
        qreal x = startRect.x() * (1-t) + endRect.x() * t;
        qreal y = startRect.y() * (1-t) + endRect.y() * t;
        QRectF currentRect(x, y, startRect.width(), startRect.height());
        view->setSceneRect(currentRect);

        // 更新图片位置，使其始终在视口中心
        int imgW = wonItem->pixmap().width();
        int imgH = wonItem->pixmap().height();
        QPointF center = currentRect.center();
        wonItem->setPos(center.x() - imgW/2, center.y() - imgH/2);
    });

    timeLine->start();

    // 可以在动画结束后 emit GameOver
    connect(timeLine, &QTimeLine::finished, this, [=](){
         timeLine->deleteLater();
         emit GameSuccess(false);
         emit GameOver();
    });
}

void GameScene::showPlayerWon(QPointF pos){
    // 停止波次计时
    if(waveTimer->isActive()) waveTimer->stop();

    // 停止背景音乐
    bgMus->stop();

    // 播放胜利音乐
    // playSoundEffect("qrc:/res/GameRes/audio/winmusic.mp3");

    // 清除所有僵尸
    emit GamePause();

    // 创建奖杯
    Trophy *trophy = new Trophy();
    addItem(trophy);

    // 点击奖杯后播放一个动画，然后没有了。

    // 最后一个僵尸死亡的位置
    trophy->setPos(pos);
    trophy->setZValue(5); // 确保在最上层

    connect(trophy, &Trophy::victoryAnimationFinished, this, &GameScene::GameOver);

    //记录setting，又解锁新关卡
    settings->beginGroup("LevelInfo");
    int currUnlock = settings->value("unlockedLevel").toInt();
    int limit = settings->value("limitLevel").toInt();
    if(currUnlock<limit
        && levelData->eName == QString::number(currUnlock))settings->setValue("unlockedLevel",currUnlock+1);
    settings->endGroup();





}



//得到僵尸
QList<Zombie*> GameScene::getZombies(){
    QList<Zombie*> res = filterGameScene<Zombie>([=](Zombie* ){
        return true;
    });
    return res;
}
QList<Zombie*> GameScene::getZombiesRow(int r){
    QList<Zombie*> res = filterGameScene<Zombie>([=](Zombie * zombie){
        return Coordinate().getRow(zombie->y()) == r;
    });

    return res;
}


//播放短时音效,use cache
void GameScene::playSoundEffect(const QString& soundPath){
    if(soundManager){
        soundManager->playSoundEffect(soundPath);
    }
}
