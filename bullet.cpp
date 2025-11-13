#include "bullet.h"
#include"animate.h"
#include<QLineF>
int Bullet::bulletNum = 0;
int Bullet::delNum = 0;

Bullet::Bullet(QString objPath,int power)
    :MyObject(nullptr,objPath,Type::Bullet),power(power),speed(200)// p/s
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
            isDead = true;//检验是否发生碰撞
            disconnect(colli_conn);  // 找到第一个僵尸后disconnect
        }
    });
    connect(this->timer,&QTimer::timeout,this,[=](){
        if (isDead) {
            if (scene()) {
                scene()->removeItem(this);
            }
            deleteLater();
            return;
        }

        // 超出场景边界时删除
        // 获取场景边界
        qreal sceneWidth = scene()->width();
        qreal sceneHeight = scene()->height();
        qreal buffer = 100; // 缓冲值，可根据需求调整

        //超出场景左、右、上、下边界
        bool outOfLeft = x() < -buffer;          // 左边界外
        bool outOfRight = x() > sceneWidth + buffer; // 右边界外
        bool outOfTop = y() < -buffer;           // 上边界外
        bool outOfBottom = y() > sceneHeight + buffer; // 下边界外

        if (outOfLeft || outOfRight || outOfTop || outOfBottom) {
            scene()->removeItem(this);

            deleteLater();
        }
    });
}
void Bullet::DealZombie(Zombie *zombie){
    if(zombie && zombie->getObjType() == Type::ZOMBIE && !zombie->IsDead())
     zombie->beHeated(power,DieType::Normal);
}



Bullet::~Bullet(){
    delNum++;
    qDebug()<<"out"+QString::number(delNum);
}




