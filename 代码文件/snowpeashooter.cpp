#include "snowpeashooter.h"
#include"snowpea.h"
#include"animate.h"
#include<QGraphicsScene>
#include<QGraphicsRectItem>

SnowPeashooter::SnowPeashooter(QGraphicsObject *parent)
    :Plant(parent,":/res/GameRes/images/SnowPea.gif",100),bulletHz(34)
{

    //单独的射程检测区域
    m_detectTimer = new QTimer();

    m_detectArea = new QGraphicsRectItem(0,-20,700,50,this);


    m_detectArea->setOpacity(0); // 设为透明不可见
    m_detectArea->setFlag(QGraphicsItem::ItemHasNoContents, false);
    // 关闭检测区域的物理碰撞
    m_detectArea->setAcceptedMouseButtons(Qt::NoButton);
    m_detectArea->setEnabled(false);

    // 监听检测区域的碰撞
    connect(m_detectTimer, &QTimer::timeout, this, [=](){
        QList<QGraphicsItem*> inRangeItems = m_detectArea->collidingItems();
        for (auto it = inRangeItems.begin();it != inRangeItems.end();it++) {
            MyObject *zombie = dynamic_cast<MyObject*>(*it);//如果能转化，说明时僵尸
            if ( zombie && zombie->getObjType() == Type::ZOMBIE) {

                emit findZombie(zombie);
                break;//发现后发送一次，避免连发
            }
        }
    });
    m_detectTimer->start(1500 / bulletSpeedRate);//加快攻速
    //
    SnowPeashooter::plantAction();
}

void SnowPeashooter::plantAction(){

    connect(this,&Plant::findZombie,this,[=](MyObject *zombie){

        if(scene() && Bullet::bulletNum<Bullet::delNum+200){
            SnowPea *bullet = new SnowPea(":/res/GameRes/images/Plants/PB-10.gif"
                                          ,15*bulletPowerRate);

            connect(this,&MyObject::GameOver,bullet,&MyObject::GameOver);//

            bullet->setPos(this->scenePos()+QPointF(40,-20));

            Animate(bullet).speed(AnimationType::Move,bullet->getSpeed() * bulletSpeedRate)
                .move(QPointF(900,0));

            scene()->addItem(bullet);
        }

    });

}


SnowPeashooter::~SnowPeashooter(){
    delete m_detectTimer;
}
