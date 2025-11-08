#include "cherrybomb.h"

#include<QGraphicsScene>
#include<QGraphicsEllipseItem>

CherryBomb::CherryBomb(QGraphicsObject *parent)
    :Plant(parent,":/res/GameRes/images/CherryBomb.gif",9999),power(999999)
{

    //单独的射程检测区域（仅用于检测僵尸，不影响物理碰撞）
    m_detectArea = new QGraphicsRectItem(-150,-125,275,250,this);

    m_detectArea->setOpacity(0); // 设为透明不可见
    m_detectArea->setFlag(QGraphicsItem::ItemHasNoContents, false);
    // 关闭检测区域的物理碰撞（不参与场景的默认碰撞逻辑）
    m_detectArea->setAcceptedMouseButtons(Qt::NoButton);
    m_detectArea->setEnabled(false);

    // 3. 监听检测区域的碰撞（仅用于检测僵尸进入）
    QTimer::singleShot(2000, this, [=](){
                emit findZombie(nullptr);
    });
    //
    CherryBomb::plantAction();
}

// 碰撞区域（仅躯干部分）
QPainterPath CherryBomb::shape() const {
    QPainterPath path;
    path.addRect(-50,-40,100,80);


    return path;
}

void CherryBomb::plantAction(){

    QMetaObject::Connection FirstConn;
    FirstConn = connect(this,&Plant::findZombie,this,[=](){
        //
        QList<QGraphicsItem*> items = m_detectArea->collidingItems();
        for (auto it = items.begin(); it != items.end(); ++it) {
            // 修正：解引用迭代器获取QGraphicsItem*
            Zombie* zombie = dynamic_cast<Zombie*>(*it);
            if (zombie && zombie->getObjType() == Type::ZOMBIE) {
                zombie->beHeated(power,DieType::CherryBOMB);
            }
        }
        disconnect(FirstConn);
        //
        isDead = true;
        changeGif(":/res/GameRes/images/Boom.gif");
        QTimer::singleShot(1200,this,[=](){
            emit Plant::dead();
            this->deleteLater();
        });
        //
    });

}

CherryBomb::~CherryBomb(){
    delete m_detectArea;
}

