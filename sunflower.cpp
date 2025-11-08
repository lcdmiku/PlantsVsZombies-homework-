#include "sunflower.h"



SunFlower::SunFlower(QGraphicsObject *parent)
    :Plant(parent,":/res/GameRes/images/SunFlower.gif",50),sunlightInterval(500)
{
    timer = new QTimer();
    timer->start(sunlightInterval);

    plantAction();
}

void SunFlower::plantAction(){
    //sunlight generate

    connect(timer,&QTimer::timeout,[=](){
        int gen = QRandomGenerator::global()->bounded(1,13);
        int x = QRandomGenerator::global()->generateDouble()*10;
        int y = QRandomGenerator::global()->generateDouble()*10+20;
        if(gen == 1){
            SunLight *sunlight = new SunLight;
            sunlight->setPos(mapToScene(QPointF(x,y)));
            emit sunlightProduce(sunlight);
            scene()->addItem(sunlight);
        }
    });
}

SunFlower::~SunFlower(){
    delete timer;
}
