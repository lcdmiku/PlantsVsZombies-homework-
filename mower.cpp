#include "mower.h"
#include<QLineF>

Mower::Mower()
    :MyObject(nullptr,":/res/GameRes/images/LawnMower.png",Type::Mower),power(99999),hasCollided(false)
{
    //刷新频率
    Hz = 30;
    speed = 200;// p/s
    moveTimer = new QTimer();
    connect(moveTimer,&QTimer::timeout,this,[=](){
        advance(1);
    });
    moveTimer->start(1000/Hz);

}

void Mower::advance(int phase)
{
    if (!phase) return;  // 跳过阶段0，只在阶段1执行更新

    // 如果有僵尸向前移动
    if(hasCollided)
    setPos(x() + speed/Hz, y());

    QList<QGraphicsItem*> items = collidingItems();

    for (auto it = items.begin(); it != items.end(); ++it) {
        // 修正：解引用迭代器获取QGraphicsItem*
        Zombie* zombie = dynamic_cast<Zombie*>(*it);
        if (zombie && zombie->getObjType() == Type::ZOMBIE && !zombie->IsDead()) {
            zombie->beHeated(power,DieType::Normal);
            hasCollided = true;
             // 找到第一个僵尸后不退出循环
        }
    }


    // 超出场景边界时删除（添加安全性检查）
    if (scene() && x() > scene()->width() + 100) {
        scene()->removeItem(this);
        moveTimer->stop();
        moveTimer->disconnect(this);
        deleteLater();
    }
}

Mower::~Mower(){
    delete moveTimer;
}




