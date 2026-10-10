#include "sunflower.h"



SunFlower::SunFlower(QGraphicsObject *parent)
    :Plant(parent,":/res/GameRes/images/Plants/SunFlower/SunFlower1.gif",80),sunlightInterval(4000)
{
    timer = new QTimer();
    timer->start(sunlightInterval);

    SunFlower::plantAction();
}

void SunFlower::plantAction(){
    //sunlight generate

    connect(timer,&QTimer::timeout,this,[=](){
        int gen = QRandomGenerator::global()->bounded(1,13);
        int x = QRandomGenerator::global()->generateDouble()*10;
        int y = QRandomGenerator::global()->generateDouble()*10+20;
        if(gen == 1){
            CurrentGif = ":/res/GameRes/images/Plants/SunFlower/SunFlower2.gif";
            ToCurrentGif();
            QTimer::singleShot(1500,this,[=](){
                SunLight *sunlight = new SunLight;
                sunlight->setPos(mapToScene(QPointF(x,y)));
                emit sunlightProduce(sunlight);
                scene()->addItem(sunlight);
                //换回gif
                CurrentGif = ":/res/GameRes/images/Plants/SunFlower/SunFlower1.gif";
                ToCurrentGif();
            });

        }
    });
}

SunFlower::~SunFlower(){
    delete timer;
}
