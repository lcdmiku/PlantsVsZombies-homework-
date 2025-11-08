#include "card.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QDebug>
#include<QFileInfo>
#include<QMimeData>
#include<QDrag>
#include<QApplication>



Card::Card(QGraphicsObject *parent, QString plantpath)
    : QGraphicsObject(parent),sunlightCost(100),cooldownTime(5000),isReady(true),plantPath(plantpath)
{

    // 创建卡片背景
    pixmap = new QGraphicsPixmapItem(QPixmap(":/res/GameRes/images/Card.png"), this);
    pixmap->setPos(0,0);
    pixmap->setFlag(QGraphicsItem::ItemStacksBehindParent, true);

    // 创建植物图像

    plantpix = new QGraphicsPixmapItem(QPixmap(plantpath), pixmap);
    plantpix->setTransformationMode(Qt::SmoothTransformation);
    int card_w = pixmap->pixmap().width();
    int plant_w = plantpix->pixmap().width();
    plantpix->setPos((card_w-plant_w)/2, 25);

    // 启用鼠标事件和悬停事件
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    //确认plant
    QString plantName = QFileInfo(plantpath).baseName();

    if(plantName == "Peashooter"){
        plantType = PlantType::PEASHOOTER;
        sunlightCost = 150;
    }
    else if(plantName == "SunFlower"){
        plantType = PlantType::SUNLIGHT;
        sunlightCost = 50;
    }
    else if(plantName == "WallNut"){
        plantType = PlantType::WALLNUT;
        sunlightCost = 100;
    }
    else if(plantName == "CherryBomb"){
        plantType = PlantType::CHEERYBOMB;
        sunlightCost = 150;
    }
    else if(plantName == "PotatoMine"){
        plantType = PlantType::POTATOMINE;
        sunlightCost = 25;
    }
    else if(plantName == "SnowPea"){
        plantType = PlantType::SNOWPEASHOOTER;
        sunlightCost = 200;
    }
    else {
        plantType = PlantType::PEASHOOTER;
    }

    //创建价格文本
    sunlightText = new QGraphicsSimpleTextItem(pixmap);
    sunlightText->setText(QString::number(sunlightCost));
    sunlightText->setFont(QFont("Arial", 20, QFont::Bold));
    sunlightText->setBrush(Qt::black);
    sunlightText->setPos(15,108);
}

QRectF Card::boundingRect() const {
    return pixmap->mapRectToParent(pixmap->boundingRect());
}

void Card::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // 如果冷却中，绘制半透明遮罩和进度条
    if (!isReady) {
        //半透明遮罩（覆盖卡片，表示不可点击）
        painter->fillRect(boundingRect(), QColor(0, 0, 0, 50)); // 黑色半透明

        //进度条（从下往上减少，对应冷却进度）
        qreal progress = m_cooldownProgress;

        QRectF barRect(
            boundingRect().left(),
            boundingRect().top(), // 顶部对齐
            boundingRect().width(),
            boundingRect().height() * progress  // 进度条宽度随进度变化
            );
        painter->fillRect(barRect, QColor(0, 0, 0, 100)); // 黑色进度条
    }

}

void Card::CoolDown(){
    isReady = false;
    // 创建冷却进度动画
    QPropertyAnimation *cooldownAnim = new QPropertyAnimation(this, "cooldownProgress");
    cooldownAnim->setDuration(cooldownTime);
    cooldownAnim->setStartValue(1.0); // 100%
    cooldownAnim->setEndValue(0.0);   // 0%

    connect(cooldownAnim, &QPropertyAnimation::finished, this, [this]() {
        isReady = true; // 冷却完成
        update();
    });

    cooldownAnim->start(QAbstractAnimation::DeleteWhenStopped);
}

Card::~Card() {

}

//拖拽种植
void Card::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    if (event->button() == Qt::LeftButton) {
        m_dragStartPosition = event->scenePos();
        qDebug() << "Card pressed";
        emit cardPress(this);//发送点击消息
        // 调用父类实现，确保选择状态正确
        QGraphicsObject::mousePressEvent(event);
    }
}

void Card::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    // 检查是否按住左键且移动距离达标
    if (!(event->buttons() & Qt::LeftButton) || !IsReady()) return;
    if ((event->pos() - m_dragStartPosition).manhattanLength() < QApplication::startDragDistance()) return;

    // 启动拖拽核心代码
    QDrag *drag = new QDrag(scene()->parent());
    QMimeData *mimeData = new QMimeData;

    // 仅传递必要的植物类型数据
    mimeData->setData("plant/type", QByteArray::number(static_cast<int>(plantType)));
    mimeData->setData("plant/cost",QByteArray::number(sunlightCost));
    drag->setMimeData(mimeData);

    // 执行拖拽
    drag->setPixmap(plantpix->pixmap());
    drag->setHotSpot(QPoint(30,30));
    drag->exec(Qt::CopyAction);

    // 调用父类实现
    QGraphicsObject::mouseMoveEvent(event);
}





