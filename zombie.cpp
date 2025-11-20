#include "zombie.h"
#include"animate.h"
#include<QLineF>
#include<QGraphicsItem>
#include<QTimer>

Zombie::Zombie(QString objPath,
               enum ZombieType zombieType,QString attackingGif,int hp,int speed,int attackpower)
    :MyObject(nullptr,objPath,Type::ZOMBIE),
    hp(hp),speed(speed),CurrentSpeedRate(1),attackPower(attackpower),attackingGif(attackingGif),zombieType(zombieType),
    Hz(30),movable(true),slowEffect(0)//81,94
{

    currentHp = hp;
    attackedPlant = nullptr;
    zombieAttackTimer = new QTimer(this);
    IfwinTimer = new QTimer(this);
    //
    hitEffect = new QGraphicsColorizeEffect(this);
    hitEffect->setColor(Qt::red);  // 设置为红色
    // 初始强度为0（无效果）
    hitEffect->setStrength(0.0);
    setGraphicsEffect(hitEffect);




    setPos(startPos);

    //与植物相撞发生
    connect(this,&MyObject::Meet,this,[=](MyObject *obj){
        Plant *plant = dynamic_cast<Plant*>(obj);
        if(plant && plant->getObjType() == Type::PLANT)
        Action(plant);
    });
    
    //检测是否到达左侧阈值
    connect(IfwinTimer,&QTimer::timeout,this,[=](){
        qDebug() << "zombie x:" << this->pos().x();
        if(this->pos().x() < 50 && !isDead){
            emit zombieSuccess();
            isDead = true; // 防止重复触发
        }
    });
    IfwinTimer->start(100);

    //攻击
    connect(zombieAttackTimer,&QTimer::timeout,this,[=](){
        if(attackedPlant)
            attackedPlant->Attacted(attackPower);
    });
    //同步计时器 ms

}



Zombie::~Zombie(){

}

void Zombie::attack(Plant *plant){
    //如果没有正在攻击的植物，然后来信号了
    if(!attackedPlant){
        attackedPlant = plant;//弱指针管理
        this->stopMoving();
        connect(attackedPlant,&Plant::dead,this,[=](){
            zombieAttackTimer->stop();
            this->continueMoving();
        });
        //启动定时器
        zombieAttackTimer->start(1000);
    }
}

void Zombie::setCurrentGif(){
    if(attackedPlant)CurrentGif = attackingGif;
}
void Zombie::stopMoving() {
    movable = false;
    Animate(this).stop(AnimationType::Move);

}

void Zombie::continueMoving() {
    movable = true;
    Animate(this).speed(AnimationType::Move,speed).move(QPointF(-1800,0));
}

// QRectF Zombie::boundingRect() const{
    // QRectF rawRect = MyObject::boundingRect();

    // rawRect.setX(-24);
    // rawRect.setRight(75);
    // rawRect.setY(-55);
    // return rawRect;
    // if(movie && movie->state() == QMovie::Running){
    //     return QRectF(0,0,movie->currentPixmap().width(),movie->currentPixmap().height());
    // }
    // else return QRectF(0,0,50,50);
// }
// 碰撞区域（仅躯干部分）
QPainterPath Zombie::shape() const {
    QPainterPath path;
    path.addRect(-20,-10,5,80);


    return path;
}

void Zombie::beHeated(int power,enum DieType dieType){
    //设置power
    currentHp -= power;
    if(currentHp <= 0 && !isDead){
        emit dead();
        stopMoving();
        isDead = true;
        dealDead(dieType);
    }
    else if(!isDead){
        hitEffect->setStrength(0.7);
        QTimer::singleShot(100,this,[=](){
            if(!slowEffect)
            hitEffect->setStrength(0.0);
        });
    }
}

void Zombie::dealDead(enum DieType dieType){
    if(dieType == DieType::CherryBOMB)
    {
        changeGif(":/res/GameRes/images/Burn.gif");
        QMetaObject::Connection Conn;
        Conn = connect(this,&MyObject::movieFinished,this,[=](){
            disconnect(Conn);
            deleteLater();
        });
    }
    else {
        QString deadGif = ":/res/GameRes/images/ZombieDie.gif";

        switch (this->zombieType) {
        case ZombieType::FootballZombie:
        {
            deadGif = ":/res/GameRes/images/FootballZombieDie.gif";
            break;
        }
        default:
            break;
        }
        changeGif(deadGif);
        secondaryDyingMovie = new QMovie(":/res/GameRes/images/ZombieHead.gif",QByteArray(),this);
        secondaryGifItem = new QGraphicsPixmapItem(this);

        if(secondaryDyingMovie->isValid()){
            secondaryDyingMovie->setCacheMode(QMovie::CacheAll);
            connect(secondaryDyingMovie,&QMovie::frameChanged,this,[=](){
                secondaryGifItem->setPixmap(secondaryDyingMovie->currentPixmap());
            });
            secondaryGifItem->setPos(this->boundingRect().x(),this->boundingRect().y());

            secondaryDyingMovie->start();
        }
        // 定时结束死亡动画
        QTimer::singleShot(2000, this, [=](){
            deleteLater();
        });
    }
}

void Zombie::setSpeed(double rate,int duration){
    CurrentSpeedRate = rate;
    if(movable)Animate(this).speed(AnimationType::Move,CurrentSpeedRate * speed).move(QPointF(-900,0));
    slowEffect++;
    hitEffect->setColor(Qt::blue);
    QTimer::singleShot(duration,this,[=](){
        slowEffect--;
        if(!slowEffect)
        {
            CurrentSpeedRate = 1;
            if(movable)Animate(this).speed(AnimationType::Move,CurrentSpeedRate * speed).move(QPointF(-900,0));
            hitEffect->setStrength(0.0);
            hitEffect->setColor(Qt::red);
        }
    });

}
//81 94
void Zombie::changeRow(enum Direction dir){

}
