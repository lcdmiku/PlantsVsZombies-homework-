#include "bullet.h"
#include"animate.h"
#include<QLineF>
int Bullet::bulletNum = 0;
int Bullet::delNum = 0;

Bullet::Bullet(QString objPath,int power)
    :MyObject(nullptr,objPath,Type::Bullet),power(power),speed(200),bomb()// p/s
    ,bombSound("qrc:/res/GameRes/audio/splat3.wav")
{
    bulletNum++;
    qDebug()<<QString::number(bulletNum);
    Hz = 30;

    //不再在bullet类内管理子弹轨迹，而是交给拿枪的植物
    //类内负责子弹击中或出界逻辑
    QMetaObject::Connection colli_conn ;
    colli_conn = connect(this,&MyObject::Meet,this,[=](MyObject* obj){
        Zombie* zombie = dynamic_cast<Zombie*>(obj);
        if (zombie && zombie->getObjType() == Type::ZOMBIE && !zombie->IsDead()) {
            DealZombie(zombie);
            dealBomb();//检验是否发生碰撞
            disconnect(colli_conn);  // 找到第一个僵尸后disconnect
        }
    });

    connect(this->timer,&QTimer::timeout,this,[=](){
        // 超出场景边界时删除
        // 获取场景边界
        GameScene* gamescene = getGameScene();
        if(gamescene)
        {
            qreal sceneWidth = scene()->width();
            qreal sceneHeight = scene()->height();
            qreal buffer = 100; // 缓冲值

            //超出场景左、右、上、下边界
            bool outOfLeft = x() < -buffer;
            bool outOfRight = x() > sceneWidth + buffer;
            bool outOfTop = y() < -buffer;
            bool outOfBottom = y() > sceneHeight + buffer;

            if (outOfLeft || outOfRight || outOfTop || outOfBottom) {
                dealBomb();
            }
        }
    });
}
void Bullet::DealZombie(Zombie *zombie){
    if(zombie && zombie->getObjType() == Type::ZOMBIE && !zombie->IsDead())
     zombie->beHeated(power,DieType::Normal);
}

void Bullet::dealBomb(){
    if(!isDead)
    {
        isDead = true;
        setCurrentGif();
        ToCurrentGif();
        playSoundEffect(bombSound);
        QTimer::singleShot(100,this,[=](){
            if (scene()) {
                scene()->removeItem(this);
            }
            deleteLater();
        });
    }
}

Bullet::~Bullet(){
    delNum++;
    qDebug()<<"out"+QString::number(delNum);
}




