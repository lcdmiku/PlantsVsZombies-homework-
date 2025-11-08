#include "shovel.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include<QDebug>
#include<QMimeData>
#include<QDrag>

Shovel::Shovel()
    :QGraphicsPixmapItem(QPixmap(":/res/GameRes/images/Shovel.png")),isSelected(false),startPos(750,5)
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
    // 启用鼠标跟踪
    setAcceptHoverEvents(true);
}

void Shovel::mousePressEvent(QGraphicsSceneMouseEvent *ev){
    isSelected = !isSelected;

}

Shovel::~Shovel(){

}
void Shovel::mouseMoveEvent(QGraphicsSceneMouseEvent *ev){
    // 检查是否按住左键且移动距离达标
    if (!(ev->buttons() & Qt::LeftButton)) return;

    // 启动拖拽核心代码
    QDrag *drag = new QDrag(scene()->parent());
    QMimeData *mimeData = new QMimeData;

    // 仅传递必要的植物类型数据
    mimeData->setData("shovel/remove", QByteArray::number(static_cast<bool>(true)));

    drag->setMimeData(mimeData);

    // 执行拖拽
    drag->setPixmap(pixmap());
    drag->setHotSpot(QPoint(30,30));
    drag->exec(Qt::CopyAction);

    // 调用父类实现
    QGraphicsItem::mouseMoveEvent(ev);
}
void Shovel::hoverMoveEvent(QGraphicsSceneHoverEvent *ev){


}
