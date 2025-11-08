#include "startwidget.h"
#include<QDebug>

StartWidget::StartWidget(QWidget *parent)
    : QWidget(parent)
{
    //开始页面
    setAutoFillBackground(true);
    setFixedSize(900,600);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(QPixmap(":/res/GameRes/images/enterbg.jpg")
                                                  .scaled(size(),
                                                          Qt::IgnoreAspectRatio,
                                                          Qt::SmoothTransformation)));
    setPalette(palette);
    start_btn = new QPushButton("start",this);
    QPalette palette_btn;
    palette_btn.setBrush(QPalette::Button,QBrush(QPixmap(":/res/GameRes/images/enterStart.png")));
    start_btn->setPalette(palette_btn);
    start_btn->setGeometry(300,450,328,93);
    connect(start_btn,&QPushButton::clicked,this,&StartWidget::startGame);

}
