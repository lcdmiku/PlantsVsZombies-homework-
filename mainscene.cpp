#include "mainscene.h"
#include "./ui_mainscene.h"
#include<QList>
#include<QVariantList>
#include<QPalette>
#include<QFile>



MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene),shop(nullptr),scene(nullptr),view(nullptr),timer(nullptr),selectPlant(nullptr)
{
    //设置标题
    setWindowTitle("Plants Fight Zombies");
    setAcceptDrops(true);

    timer = new QTimer(this);
    timer->start(1000);
    timer->stop();

    moveTimer = new QTimer(this);
    moveTimer->start(30);
    moveTimer->stop();

    //处理游戏暂停和继续
    connect(this,&MainScene::GamePause,this,[=](){
        if(moveTimer->isActive())moveTimer->stop();
        if(timer->isActive())timer->stop();

    });
    connect(this,&MainScene::GameOver,this,[=](){
        if(moveTimer->isActive())moveTimer->stop();
        if(timer->isActive())timer->stop();
        if(shovel){
            delete shovel;
            shovel = nullptr;
        }
    });
    connect(this,&MainScene::GameContinue,this,[=](){
        if(!moveTimer->isActive())moveTimer->start();
        if(!timer->isActive())timer->start();
    });

    ui->setupUi(this);
    //显示主场景
    setFixedSize(900,600);

    //初始化场景
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);
    startWidget = new StartWidget(this);
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene,this);
    //游戏结束后清空场景元素


    //settingsMenu
    QPushButton *menu_btn = new QPushButton("Menu",this);
    settingsMenu = new SettingsMenu(this);
    connect(menu_btn,&QPushButton::clicked,this,[=](){
        settingsMenu->exec();
    });
    QPalette palette_menubtn;
    palette_menubtn.setBrush(QPalette::Button,QBrush(QPixmap(":/res/GameRes/images/Button.png")));
    menu_btn->setPalette(palette_menubtn);
    //
    connect(settingsMenu,&SettingsMenu::GamePause,this,&MainScene::GamePause);
    connect(settingsMenu,&SettingsMenu::GameContinue,this,&MainScene::GameContinue);
    connect(settingsMenu,&SettingsMenu::getBack,this,[=](){
        if(stackedWidget->currentIndex()>0){
            stackedWidget->setCurrentIndex(stackedWidget->currentIndex()-1);
            emit GameOver();//退出就结束游戏
        }
    });


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



    //config文件
    settings = new QSettings("config.ini",QSettings::IniFormat);
    //map信息 //成功实现
    // QVariantList mapRow = {1,200,0,0,0,0,0,0,0};
    // settings->beginGroup("MapInfo");
    // for(int i=0;i<5;i++){
    //     settings->setValue(QString("row%1").arg(i),mapRow);
    // }
    // settings->endGroup();

    //利用QStackedWidget管理多页
    stackedWidget->addWidget(startWidget);
    stackedWidget->addWidget(view);
    view->setAutoFillBackground(true);
    stackedWidget->setCurrentWidget(startWidget);


    connect(startWidget,&StartWidget::startGame,this,[=](){
        if(!selectPlant)
        GamePre();
        else{
            selectPlant->setPos(290,100);
        }
        stackedWidget->setCurrentWidget(view);
     });

}


void MainScene::cardAvailable(){
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



void MainScene::GamePre(){

    scene->setSceneRect(150, 0, 900, 600);
    //设置选择板

    selectPlant = new SelectPlant();
    scene->addItem(selectPlant);
    //给选择板添加卡片
    cardAvailable();
    selectPlant->setPos(290,100);
    //设置背景
    QGraphicsPixmapItem *gameBg = new QGraphicsPixmapItem(QPixmap(":/res/GameRes/images/Background.jpg"));
    gameBg->setPos(-330,0);
    gameBg->setZValue(-100);
    scene->addItem(gameBg);

    //设置开始按键
    QPushButton *startBtn = new QPushButton("Start");
    QPalette palette_Btn;//按键统一样式
    palette_Btn.setBrush(QPalette::Button,QBrush(QPixmap(":/res/GameRes/images/Button.png")));
    startBtn->setPalette(palette_Btn);
    startBtn->resize(100,40);
    QGraphicsProxyWidget *start_proxy = scene->addWidget(startBtn);
    //设置按钮在场景中的位置
    start_proxy->setPos(900,500);
    //设置卡片清空按键
    QPushButton *cardDeleteBtn = new QPushButton("Clear Cards");
    cardDeleteBtn->setPalette(palette_Btn);
    cardDeleteBtn->resize(100,40);
    QGraphicsProxyWidget *cardDelete_proxy = scene->addWidget(cardDeleteBtn);
    //设置按钮在场景中的位置
    cardDelete_proxy->setPos(900,400);

    //设置场景移动动画
    connect(moveTimer,&QTimer::timeout,this,[=](){
        if(gameBg->x()<0){
            gameBg->setPos(gameBg->x()+10,0);
        }
        //selectpalnt
        if(selectPlant->y()>-400){
            selectPlant->setPos(290,selectPlant->y()-10);
        }
    });

    connect(startBtn,&QPushButton::clicked,this,[=](){
        emit GameContinue();
        start_proxy->hide();
        cardDelete_proxy->hide();
        //设置场景移动动画
        QTimer::singleShot(1200,this,[=](){
            GameStart();
        });

    });
    connect(this,&MainScene::GameOver,this,[=](){
        start_proxy->show();
        cardDelete_proxy->show();
    });


    scene->setItemIndexMethod(QGraphicsScene::NoIndex);


        shop = new Shop;
        //选择卡片逻辑
        connect(selectPlant,&SelectPlant::cardPress,shop,[=](Card *card){
            shop ->addCard(card->getPlantPath());
        });
        connect(cardDeleteBtn,&QPushButton::clicked,this,[=](){
            if(shop)shop->clearCards();
            selectPlant->reSet();
        });
        connect(this,&MainScene::GameOver,shop,[=](){
            if(shop)shop->clearCards();
            selectPlant->reSet();
        });

        shop->setPos(290, 0);
        scene->addItem(shop);




    view->setFixedSize(902, 602);
    view->setRenderHint(QPainter::Antialiasing);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->show();


}

void MainScene::GameStart(){

    //shovel
    shovel = new Shovel;

    shovel->setPos(shovel->getStartPos());
    scene->addItem(shovel);
    // connect(this,&MainScene::GameOver,this,[=](){
    //     if(shovel){
    //         scene->removeItem(shovel);
    //         delete shovel;
    //         shovel = nullptr;
    //     }
    // });
    // //menu
    // QPushButton *menuBtn = new QPushButton("Menu");
    // QGraphicsProxyWidget *proxy = scene->addWidget(menuBtn);
    // // 可以设置按钮在场景中的位置
    // proxy->setPos(900,0);
    // connect(menuBtn,&QPushButton::clicked,this,[=](){
    //     settingsMenu->exec();
    // });
    // QPalette palette_menubtn;
    // palette_menubtn.setBrush(QPalette::Button,QBrush(QPixmap(":/res/GameRes/images/Button.png")));
    // menuBtn->setPalette(palette_menubtn);

    //sunlight generate
    connect(timer,&QTimer::timeout,this,[=](){
        int gen = QRandomGenerator::global()->bounded(1,20);
        int x = QRandomGenerator::global()->generateDouble()*this->width();
        int y = QRandomGenerator::global()->generateDouble()*this->height();
        if(gen == 1){
            SunLight *sunlight = new SunLight;
            sunlight->setPos(x,y);
            //仅仅收集阳光使用bool(int)
            bool (Shop::*func)(int) = &Shop::sunlightValueShow;
            connect(sunlight,&SunLight::sunlightCollected,shop,func);

            scene->addItem(sunlight);
            connect(this,&MainScene::GameOver,sunlight,&QGraphicsObject::deleteLater);
        }
    });

    //zombie generate
    QMetaObject::Connection Conn;
    Conn = connect(timer,&QTimer::timeout,this,[=](){
        int gen = QRandomGenerator::global()->bounded(0,1);//僵尸数量越大，越不用以生成
        if(gen == 0)
        {
            this->ZombieGenerate();
        }
    });

    //add card
    //cardADD();

    //plantarea
    PlantAreaGenerate();

    //zombie generate
    ZombieGenerate();

}

void MainScene::PlantAreaGenerate(){

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
            scene->addItem(mower);
            connect(this,&MainScene::GameOver,mower,&MyObject::GameOver);
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
            PlantArea *area = new PlantArea(landType,moveTimer);
            area->setPos(QPointF(150 +105 ,90) + QPointF(area->w()*j , area->h()*i));//81,94
            //连接向日葵生成的阳光
            connect(area,&PlantArea::sunlightProduced,this,[=](SunLight *sunlight){
                //仅仅收集阳光使用bool(int)
                bool (Shop::*func)(int) = &Shop::sunlightValueShow;
                connect(sunlight,&SunLight::sunlightCollected,shop,func);
                connect(this,&MainScene::GameOver,sunlight,&QGraphicsObject::deleteLater);
            });
            //买并种植植物消耗阳光
            connect(area,&PlantArea::needToPlant,this,[=](int cost,enum PlantType plantType){
                //如果阳光足够
                if(shop->sunlightValueShow(-cost,plantType)){
                    area->plant(plantType);
                }
            });
            scene->addItem(area);
            connect(this,&MainScene::GameOver,area,&PlantArea::GameOver);

        }
    }
    //
    settings->endGroup();
}

//生成僵尸
void MainScene::ZombieGenerate(){
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
    QPointF end(100,y);
    //
    int gen = QRandomGenerator::global()->bounded(0,11);//随机0到4
    Zombie *zombie=nullptr;
    //随机决定僵尸种类
    if(gen<3)
        zombie = new NomalZombie(start,end);
    else if(gen<5){
        zombie = new ScreenZombie(start,end);
    }
    else if(gen<7){
        zombie = new BucketZombie(start,end);
    }
    else if(gen<9){
        zombie = new ConeZombie(start,end);
    }
    else if(gen<11){
        zombie = new FootballZombie(start,end);
    }
    if(zombie)
    {
        scene->addItem(zombie);
        //处理僵尸行走
        connect(moveTimer,&QTimer::timeout,zombie,[=](){
            zombie->proceed();
        });
        //处理僵尸胜利的请款
        connect(zombie,&Zombie::zombieSuccess,this,[=](){

        });
        connect(this,&MainScene::GameOver,zombie,&MyObject::GameOver);
    }
}

MainScene::~MainScene()
{
    delete timer;
    delete settings;
    delete ui;
}
