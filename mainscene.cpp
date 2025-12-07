#include "mainscene.h"
#include "./ui_mainscene.h"
#include<QList>
#include<QVariantList>
#include<QPalette>
#include<QFile>
#include"animate.h"

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene),scene(nullptr),view(nullptr)
{
    //初始化settings
    settingInit();
    //设置标题
    setWindowTitle("Plants Fight Zombies");
    setAcceptDrops(true);

    ui->setupUi(this);
    //显示主场景
    setFixedSize(900,600);

    // 初始化 StackedWidget
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // 初始化 StartWidget
    startWidget = new StartWidget(this);
    stackedWidget->addWidget(startWidget);

    // 初始化 SelectLevelWidget
    selectLevelWidget = new SelectLevelWidget(this);
    stackedWidget->addWidget(selectLevelWidget);

    // 初始化 GameScene 和 View
    // 延迟到关卡选择时创建
    scene = nullptr;
    view = nullptr;
    // stackedWidget->addWidget(view);

    //settingsMenu
    QPushButton *menu_btn = new QPushButton("Menu",this);
    settingsMenu = new SettingsMenu(this);
    // scene->setMenu(settingsMenu);

    // 监听游戏结束信号（包括从菜单返回）
    // 移到创建场景时连接

    connect(menu_btn,&QPushButton::clicked,this,[=](){
        settingsMenu->exec();
    });
    QPalette palette_menubtn;
    palette_menubtn.setBrush(QPalette::Button,QBrush(QPixmap(":/res/GameRes/images/Button.png")));
    menu_btn->setPalette(palette_menubtn);
    menu_btn->move(820, 10); // 设置菜单按钮位置
    menu_btn->hide(); // 初始隐藏

    // 默认显示 StartWidget
    stackedWidget->setCurrentWidget(startWidget);

    // 连接信号
    connect(startWidget, &StartWidget::startGame, this, [=](){
        stackedWidget->setCurrentWidget(selectLevelWidget);
    });

    connect(selectLevelWidget, &SelectLevelWidget::levelSelected, this, [=](int level){
        // 创建新的 GameScene
        GameLevelData* leveldata = GameLevelDataFactory(QString::number(level));//先得到leveldata
        if(!leveldata){
            qDebug()<<"gameleveldata is nullptr";
            leveldata = GameLevelDataFactory("1");
        }
        if(scene) {
            delete scene;
            scene = nullptr;
        }
        scene = new GameScene(this,leveldata);
        connect(scene,&GameScene::GameSuccess,selectLevelWidget,[=](){
            selectLevelWidget->addLevelButton();
        });
        scene->setMenu(settingsMenu);

        // 创建新的 View
        if(view) {
            delete view;
            view = nullptr;
        }
        view = new QGraphicsView(scene);
        stackedWidget->addWidget(view); // 添加到栈中

        // 设置 View 属性
        view->setFixedSize(902, 602);
        view->setRenderHint(QPainter::Antialiasing);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setCacheMode(QGraphicsView::CacheBackground);
        view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

        // 连接 GameOver 信号
        // 连接 GameOver 信号
        // 使用 QueuedConnection 确保在事件循环的下一次迭代中处理，避免在信号发射栈中进行销毁操作
        connect(scene, &GameScene::GameOver, this, [=](){
            qDebug() << "MainScene received GameOver";
            // 防止重复调用导致崩溃
            if (!scene || !view) return;

            stackedWidget->setCurrentWidget(selectLevelWidget);
            menu_btn->hide();

            // 延迟删除场景和视图
            stackedWidget->removeWidget(view);
            
            // 安全清理：先解绑场景，再删除
            view->setScene(nullptr);
            
            // 显式清空场景中的所有图元，防止图元析构顺序问题
            scene->clear();

            // 确保 scene 销毁时不会触发 view 的重绘或其他事件
            scene->deleteLater();
            
            // view 稍后删除
            view->deleteLater();
            
            scene = nullptr;
            view = nullptr;
            qDebug() << "Scene and View scheduled for deletion";
        }, Qt::QueuedConnection);

        stackedWidget->setCurrentWidget(view);
        menu_btn->show();
        menu_btn->raise();

        // 初始化游戏
        scene->GamePre();
        scene->setSceneRect(150, 0, 900, 600);
        view->show();
    });
}





void MainScene::GamePre(){


    scene->GamePre();

    scene->setSceneRect(150, 0, 900, 600);

    view->setFixedSize(902, 602);
    view->setRenderHint(QPainter::Antialiasing);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->show();


}

void MainScene::settingInit(){

        // 如果配置中没有 MapInfo 节，写入默认配置（以 QVariantList 格式写入，便于后续 .toList() 读取）
        QSettings *settings = new QSettings("config.ini",QSettings::IniFormat);
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
        delete settings;

}
MainScene::~MainScene()
{

    delete ui;
}
