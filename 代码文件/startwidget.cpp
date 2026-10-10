#include "startwidget.h"
#include <QDebug>
#include <QPixmap>
#include <QIcon>

StartWidget::StartWidget(QWidget *parent)
    : QWidget(parent)
{
    //开始页面
    setAutoFillBackground(true);
    setFixedSize(900,600);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(QPixmap(":/res/GameRes/images/mainMenu.png")
                                                  .scaled(size(),
                                                          Qt::IgnoreAspectRatio,
                                                          Qt::SmoothTransformation)));
    setPalette(palette);
    
    start_btn = new QPushButton(this);
    QPixmap btnPix(":/res/GameRes/images/interface/SelectorAdventureButton.png");
    int w = btnPix.width();
    int h = btnPix.height() / 2;
    
    // 切割图片：上半部分为正常状态，下半部分为按下状态
    QPixmap normalPix = btnPix.copy(0, 0, w, h);
    QPixmap pressedPix = btnPix.copy(0, h, w, h);

    start_btn->resize(w, h);
    start_btn->move(470, 75); 
    
    start_btn->setIcon(QIcon(normalPix));
    start_btn->setIconSize(QSize(w, h));
    start_btn->setStyleSheet("border:none;");
    
    // 按钮按下和释放时切换图标
    connect(start_btn, &QPushButton::pressed, [=](){
        start_btn->setIcon(QIcon(pressedPix));
    });
    connect(start_btn, &QPushButton::released, [=](){
        start_btn->setIcon(QIcon(normalPix));
    });
                             
    connect(start_btn,&QPushButton::clicked,this,&StartWidget::startGame);

}
