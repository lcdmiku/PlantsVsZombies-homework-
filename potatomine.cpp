#include "potatomine.h"


PotatoMine::PotatoMine(QGraphicsObject *parent)
    :Plant(parent,":/res/GameRes/images/PotatoMine1.gif",25),power(999999),hideTime(15000),hide(true)
{

    //单独的射程检测区域（仅用于检测僵尸，不影响物理碰撞）
    m_detectArea = new QGraphicsRectItem(-50,-45,100,90,this);
    m_detectTimer = new QTimer();
    m_detectArea->setOpacity(0); // 设为透明不可见
    m_detectArea->setFlag(QGraphicsItem::ItemHasNoContents, false);
    // 关闭检测区域的物理碰撞（不参与场景的默认碰撞逻辑）
    m_detectArea->setAcceptedMouseButtons(Qt::NoButton);
    m_detectArea->setEnabled(false);


    // 3. 监听检测区域的碰撞（仅用于检测僵尸进入）
    connect(m_detectTimer,&QTimer::timeout,this,[=](){
        QList<QGraphicsItem*> items = m_detectArea->collidingItems();
        for(auto it = items.begin();it != items.end();it++){
            Zombie* zombie = dynamic_cast<Zombie*>(*it);
            if(zombie && zombie->getObjType() == Type::ZOMBIE){
                zombie->beHeated(power,DieType::CherryBOMB);
                isDead = true;
            }
        }
        if(isDead)//当有僵尸，地雷爆炸死亡
        {
            m_detectTimer->stop();
            emit findZombie(nullptr);
        }
    });
    //地雷潜伏
    QTimer::singleShot(hideTime,this,[=](){
        setUneatable();
        m_detectTimer->start(500);//潜伏结束后开始索敌
        hide = false;
    });
    PotatoMine::plantAction();
}

//
void PotatoMine::setCurrentGif(){
    if(!hide){
        CurrentGif = ":/res/GameRes/images/PotatoMine.gif";
    }

}

// 碰撞区域（仅躯干部分）
QPainterPath PotatoMine::shape() const {
    QPainterPath path;
    path.addRect(-50,-45,100,90);


    return path;
}
void PotatoMine::plantAction(){

    QMetaObject::Connection FirstConn;
    FirstConn = connect(this,&Plant::findZombie,this,[=](){
        disconnect(FirstConn);
        //
        changeGif(":/res/GameRes/images/PotatoMineBomb.gif");
        QTimer::singleShot(1200,this,[=](){
            emit Plant::dead();
            this->deleteLater();
        });
        //
    });

}

PotatoMine::~PotatoMine(){
    delete m_detectTimer;
    delete m_detectArea;
}

