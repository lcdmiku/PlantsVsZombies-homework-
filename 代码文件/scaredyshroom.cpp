#include "scaredyshroom.h"
#include"bullet.h"
#include"animate.h"
#include<QGraphicsScene>
#include<QGraphicsRectItem>
ScaredyShroom::ScaredyShroom(QGraphicsObject *parent)
    :Plant(parent,":/res/GameRes/images/Plants/ScaredyShroom/ScaredyShroomSleep.gif",80),bulletHz(34)
{

    //单独的射程检测区域
    m_detectTimer = new QTimer();
    m_detectArea = new QGraphicsRectItem(0,-20,700,50,this);
    m_detectArea->setOpacity(0); // 设为透明不可见
    m_detectArea->setFlag(QGraphicsItem::ItemHasNoContents, false);
    // 关闭检测区域的物理碰撞
    m_detectArea->setAcceptedMouseButtons(Qt::NoButton);
    m_detectArea->setEnabled(false);

    //监听检测区域的碰撞
    connect(m_detectTimer, &QTimer::timeout, this, [=](){
        QList<QGraphicsItem*> inRangeItems = m_detectArea->collidingItems();
        int flag = 0;//目前是否有僵尸 0:meiyou 1:you 2:tai jing cry
        for (auto it = inRangeItems.begin();it != inRangeItems.end();it++) {
            MyObject *zombie = dynamic_cast<MyObject*>(*it);//如果能转化，说明时僵尸
            if ( zombie && zombie->getObjType() == Type::ZOMBIE) {
                if(abs(zombie->x() - this->x()) > 500 && flag != 2)
                {
                    flag = 1;
                }
                else flag = 2;
            }
        }
        switch (flag) {
        case 0:
        {

            CurrentGif = ":/res/GameRes/images/Plants/ScaredyShroom/ScaredyShroomSleep.gif";
            break;
        }
        case 1:
        {
            emit findZombie(nullptr);
            CurrentGif = ":/res/GameRes/images/Plants/ScaredyShroom/ScaredyShroom.gif";
            break;
        }
        case 2:
        {
            CurrentGif = ":/res/GameRes/images/Plants/ScaredyShroom/ScaredyShroomCry.gif";
            break;
        }
        default:
            break;
        }

        ToCurrentGif();

    });
    m_detectTimer->start(1400);
    //
    ScaredyShroom::plantAction();
}

void ScaredyShroom::plantAction(){

    connect(this,&Plant::findZombie,this,[=](MyObject *zombie){

        if(scene() && Bullet::bulletNum<Bullet::delNum+200){
            Bullet *bullet = new Bullet(":/res/GameRes/images/Plants/ShroomBullet.gif",35*bulletPowerRate);
            connect(this,&MyObject::GameOver,bullet,&MyObject::GameOver);
            bullet->setPos(this->scenePos()+QPointF(50,5));
            bullet->setBombGif(":/res/GameRes/images/Plants/ShroomBulletHit.gif");
            Animate(bullet).speed(AnimationType::Move,bullet->getSpeed()*bulletSpeedRate).move(QPointF(900,0));
            scene()->addItem(bullet);
        }

    });

}


ScaredyShroom::~ScaredyShroom(){
    delete m_detectTimer;
}
