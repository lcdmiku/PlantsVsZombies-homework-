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

GameScene::GameScene(QObject *parent,GameLevelData* data)
    : QGraphicsScene(parent),settingsMenu(nullptr),levelData(data),
    moment(0),currWave(0),waveMoment(0),
    plantareas(),zombies(),plants(),mowers(),
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

    QGraphicsScene::addItem(shop);
    connect(this, &GameScene::GamePause, shop, &MyObject::GamePause);
    connect(this, &GameScene::GameContinue, shop, &MyObject::GameContinue);

    selectPlant = new SelectPlant();//选择板
    QGraphicsScene::addItem(selectPlant);
    connect(this, &GameScene::GamePause, selectPlant, &MyObject::GamePause);
    connect(this, &GameScene::GameContinue, selectPlant, &MyObject::GameContinue);

    shovel = new Shovel;//铲子,在~GameScene中delete
    addItem(shovel);

    gameBg = new QGraphicsPixmapItem(QPixmap(bgPath));//背景，在~GameScene中delete
    addItem(gameBg);

    dominator = new Dominator();
    QGraphicsScene::addItem(dominator);
    connect(this, &GameScene::GamePause, dominator, &MyObject::GamePause);
    connect(this, &GameScene::GameContinue, dominator, &MyObject::GameContinue);
    //音效
    audioOutput->setVolume(0.5);
    bgMus->setAudioOutput(audioOutput);
    bgMus->setLoops(-1);
    connect(settingsMenu,&SettingsMenu::volumeChanged,this,[=](int volume){
        audioOutput->setVolume(double(volume)/100);
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
        //当前波结束，开始下一波计时
        if(currWave < levelData->waveNum && waveMoment>=levelData->waveDuration[currWave]){
            setNextWave();
        }

    });
}
//处理进入下一波
void GameScene::setNextWave(){
    waveMoment = 0;
    currWave++;
    emit waveStart(currWave);//发送当前波开始的信号
    qDebug()<<"now the"<<currWave<<"-th wave";
    //当全部结束，标志胜利
    if(currWave>=levelData->waveNum){
        emit GameSuccess(true);
        showPlayerWon();
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
//选这植物阶段
void GameScene::GamePre(){
    qDebug() << "into GamePre()函数";

    //选择版
    cardAvailable();
    if(selectPlant) selectPlant->setPos(290,100);

    qDebug() << "选择版";
    //设置背景
    if(gameBg) {
        gameBg->setPos(-330,0);
        qDebug()<<gameBg->pos()<<gameBg->x()<<gameBg->y();
        gameBg->setZValue(-100);
    }

    qDebug() << "选择背景";

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


    // 之前的代码:
    /*
    connect(this,&GameScene::GameOver,this,[=](){
        start_proxy->show();
        cardDelete_proxy->show();
    });
    */
    // 移除这段，改为在 reset() 中调用 GamePre() 时自然会显示，或者在 GameOver 信号处理中不直接显示，而是等待 reset

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
    // playBGM(levelData->backgroundMusic);太吵了,先注释掉
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
    // mowerGenerate();//暂时注释，调试完放回

    //zombie generate
    // ZombieGenerate();
    //第0波开始
    emit waveStart(0);

    // DEBUG: 10秒后直接触发胜利
    // QTimer::singleShot(10000, this, [=](){
    //     qDebug() << "DEBUG: Triggering player victory.";
    //     showPlayerWon();
    // });

    //Debug: 10秒后触发僵尸胜利
    QTimer::singleShot(10000, this, [=](){
        qDebug() << "DEBUG: Triggering zombie victory.";
        showZombieWon();
    });
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
            Mower *mower = new Mower();
            mowers.insert(i,mower);//加入mower
            mower->setPos(QPointF(150 +105 ,120) + QPointF(-20 ,94*i));
            mower->setZValue(-1);
            addItem(mower);
            connect(this,&GameScene::GameOver,mower,&MyObject::GameOver);
        }
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
        zombies.push_back(zombie);
        this->zombieRow[row].push_back(zombie);
        zombie->setZValue(row);
        addItem(zombie);
        //僵尸行走
        Animate(zombie).speed(AnimationType::Move,zombie->getSpeed()).move(QPointF(-1800,0));
        //处理僵尸胜利的情况：当僵尸触发胜利时，标记为关卡失败并结束游戏
        connect(zombie,&Zombie::zombieSuccess,this,&GameScene::showZombieWon);
        connect(this,&GameScene::GameOver,zombie,&MyObject::GameOver);
        connect(this,&GameScene::GamePause,zombie,&MyObject::GamePause);
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
    default:
        break;
    }
    if(zombie)
    {
        zombie->setPos(QPointF(0 ,100) + QPointF(x ,94*row));
        zombies.push_back(zombie);
        this->zombieRow[row].push_back(zombie);
        zombie->setZValue(row);
        addItem(zombie);
        //僵尸行走
        Animate(zombie).speed(AnimationType::Move,zombie->getSpeed()).move(QPointF(-1800,0));
        //处理僵尸胜利的情况：当僵尸触发胜利时，标记为关卡失败并结束游戏
        connect(zombie,&Zombie::zombieSuccess,this,&GameScene::showZombieWon);
        connect(this,&GameScene::GameOver,zombie,&MyObject::GameOver);//与消亡绑定
        connect(this,&GameScene::GamePause,zombie,&MyObject::GamePause);
    }
}
//根据当前波数生成僵尸
void GameScene::ZombieGenerate(int currwave){
    QList<ZombieType> zombies = levelData->zombieExtract(currwave);
    for (int i = 0; i < zombies.size(); ++i)
    {
        int row = QRandomGenerator::global()->bounded(0,5);//随机投发到row行
        //随机出现时间/s,每波开始后至少6s后再刷新僵尸
        int showTime = QRandomGenerator::global()->bounded(6,levelData->waveDuration[currwave]);
        //将僵尸生存时间分开
        QTimer::singleShot(showTime*1000,this,[=](){
            ZombieGenerate(zombies[i],row,this->width()+200);
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
    // delete shovel; // 场景会自动删除这些 item
    // delete gameBg;
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
         // emit GameOver(); // 暂时不 emit GameOver，以免清空场景导致看不到图片
         emit GameSuccess(false);
         emit GameOver();
    });
}

void GameScene::showPlayerWon(){
    // 停止波次计时
    if(waveTimer->isActive()) waveTimer->stop();

    // 停止背景音乐
    bgMus->stop();

    // 播放胜利音乐
    playSoundEffect("qrc:/res/GameRes/audio/winmusic.mp3");

    // 清除所有僵尸（可选，或者定住它们）
    emit GamePause();

    // 创建奖杯
    Trophy *trophy = new Trophy();
    addItem(trophy);

    // 点击奖杯后播放一个动画，然后没有了。

    // 居中显示
    // View 的 sceneRect 是 (150, 0, 900, 600)。中心点 (600, 300)。
    // 假设图片大小适中，居中显示
    // 需要先获取 pixmap 大小，但 MyObject 默认是异步加载 movie 或者 path
    // 这里我们假设 Trophy 构造函数里已经加载了图片或者会在 paint 时绘制
    // 为了确保位置正确，我们可以手动设置一下位置，或者在 Trophy 里处理
    // 由于 MyObject 使用 QMovie 或 path，我们需要确保它有尺寸

    // 简单的居中逻辑，假设 Trophy 默认大小
    trophy->setPos(600, 300);
    trophy->setZValue(5); // 确保在最上层
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

// 走了歪路，在gamescene.cpp里写了个数据来重置数据，后来一想直接在mainscene里重建gamescene就行了，没用了。
// void GameScene::reset() {
//     // 1. 重置变量
//     currWave = 0;
//     waveMoment = 0;
//     moment = 0;
//     qDebug() << "1";

//     // 2. 停止计时器
//     if(waveTimer->isActive()) waveTimer->stop();
//     qDebug() << "2";


//     // 3. 清理僵尸
//     for(auto zombie : zombies) {
//         if(zombie) {
//             removeItem(zombie);
//             delete zombie;
//         }
//     }
//     zombies.clear();
//     zombieRow.clear();
//     qDebug() << "3";

//     // 4. 清理植物
//     for(auto plant : plants) {
//         if(plant) {
//             removeItem(plant);
//             delete plant;
//         }
//     }
//     plants.clear();
//     plantRow.clear();
//     qDebug() << "4";

//     // 5. 清理子弹、阳光等其他动态物体
//     // 由于没有专门的列表存储子弹和阳光，我们需要遍历场景中的所有 item
//     QList<QGraphicsItem*> allItems = items();

//     // 筛选出顶层 Item，避免因为删除了父 Item 导致子 Item 变成野指针，从而在后续循环中崩溃
//     QList<QGraphicsItem*> topLevelItems;
//     for(auto item : allItems) {
//         if (item->parentItem() == nullptr) {
//             topLevelItems.append(item);
//         }
//     }

//     for(auto item : topLevelItems) {
//         // 排除永久存在的对象
//         if(item == shop || item == selectPlant || item == shovel || item == gameBg || item == dominator || item == start_proxy || item == cardDelete_proxy) {
//             continue;
//         }
//         qDebug() << "5.1";
//         // 排除 SettingsMenu (它可能不是 item，但以防万一)

//         // 识别并删除 SunLight, Bullet, Mower, Trophy, ZombiesWon 图片等
//         // 使用 dynamic_cast 或 type() 判断
//         // 这里简单起见，假设 MyObject 子类除了上面排除的都需要清理

//         // 注意：Mower 是在 map 中管理的，也需要清理并重新生成
//         // Trophy, ZombiesWon 图片也需要清理

//         // 简单判断：如果不是我们保留的几个指针，就删除

//         // 如果 PlantArea 是永久的，需要保留。
//         // 检查 PlantArea 是否在 plantareas 列表中
//         // PlantAreaGenerate()函数中有连接，但是还是放在这里统一管理
//         bool isPlantArea = false;
//         for(auto pa : plantareas) {
//             if(item == pa) {
//                 isPlantArea = true;
//                 break;
//             }
//         }
//         qDebug() << "5.2";

//         if(isPlantArea) {
//             // 重置 PlantArea 状态
//             PlantArea* pa = dynamic_cast<PlantArea*>(item);
//             if(pa) pa->removePlant();
//             continue;
//         }
//         qDebug() << "5.3";

//         removeItem(item);
//         delete item;

//         qDebug() << "5.4";
//     }
//     qDebug() << "5";

//     // 6.mowers清空列表
//     mowers.clear();

//     // 7. 重置商店和选择面板
//     if(shop) {
//         shop->clearCards();
//         shop->setSun(50); // 初始阳光
//     }
//     if(selectPlant) selectPlant->reSet();
//     qDebug() << "7";

//     // 8. 重置背景位置
//     if(gameBg) gameBg->setPos(-330, 0);
//     qDebug() << "8";

//     // 9. 重置视口 (如果被 ZombiesWon 移动了)
//     QList<QGraphicsView *> views = this->views();
//     if (!views.isEmpty()) {
//         views.first()->setSceneRect(150, 0, 900, 600);
//     }
//     qDebug() << "9";

//     // 10. 重新播放背景音乐 (如果需要)
//     // playBGM("..."); // 需要知道当前关卡的 BGM
// }
