#include "bullet.h"

#include<QLineF>
int Bullet::bulletNum = 0;
int Bullet::delNum = 0;

Bullet::Bullet(QString objPath,int power,QTimer *moveTimer)
    :MyObject(nullptr,objPath,Type::Bullet),power(power),moveTimer(moveTimer)
{
    bulletNum++;
    qDebug()<<QString::number(bulletNum);
    Hz = 30;
    speed = 200;// p/s
    connect(moveTimer,&QTimer::timeout,this,[=](){
        advance(1);
    });

}
void Bullet::DealZombie(Zombie *zombie){
    if(zombie && zombie->getObjType() == Type::ZOMBIE && !zombie->IsDead())
     zombie->beHeated(power,DieType::Normal);
}

void Bullet::advance(int phase)
{
    if (!phase) return;  // 跳过阶段0，只在阶段1执行更新

    // 子弹向前移动
    setPos(x() + speed/Hz, y());

    // 检测碰撞
    QList<QGraphicsItem*> items = collidingItems();

    for (auto it = items.begin(); it != items.end(); ++it) {
        // 修正：解引用迭代器获取QGraphicsItem*
        Zombie* zombie = dynamic_cast<Zombie*>(*it);
        if (zombie && zombie->getObjType() == Type::ZOMBIE && !zombie->IsDead()) {
            DealZombie(zombie);

            isDead = true;//检验是否发生碰撞
            break;  // 找到第一个僵尸后退出循环
        }
    }

    // 在循环外处理删除，确保安全
    if (isDead) {
        if (scene()) {
            // moveTimer->stop();
            // // 只断开与moveTimer的连接，保留其他连接
            moveTimer->disconnect(this);
            scene()->removeItem(this);
        }
        deleteLater();
        return;
    }

    // 超出场景边界时删除（添加安全性检查）
    if (scene() && x() > scene()->width() + 100) {
        // moveTimer->stop();
        // // 只断开与moveTimer的连接，保留其他连接
        moveTimer->disconnect(this);
        scene()->removeItem(this);

        deleteLater();
    }
}

Bullet::~Bullet(){
    delNum++;
    qDebug()<<"out"+QString::number(delNum);
}




