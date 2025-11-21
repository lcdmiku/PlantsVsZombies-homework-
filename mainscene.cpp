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

    connect(selectLevelWidget, &SelectLevelWidget::levelSelected, this, [=](){
        // 创建新的 GameScene
        if(scene) {
            delete scene;
            scene = nullptr;
        }
        scene = new GameScene(this);
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
        connect(scene, &GameScene::GameOver, this, [=](){
            stackedWidget->setCurrentWidget(startWidget);
            menu_btn->hide();

            // 延迟删除场景和视图
            stackedWidget->removeWidget(view);
            scene->deleteLater();
            view->deleteLater();
            scene = nullptr;
            view = nullptr;
        });

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


MainScene::~MainScene()
{

    delete ui;
}
