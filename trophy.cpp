#include "trophy.h"
#include "animate.h"
#include <QDebug>
#include"coordinate.h"

Trophy::Trophy(QGraphicsObject *parent)
    : MyObject(parent, "", Type::Other), isClicked(false) // 传空路径，避免 MyObject 创建 movie
{
    if(!pixmap.load(":/res/GameRes/images/interface/trophy.png")){
        qDebug() << "Failed to load trophy.png";
        // 加载失败时，创建一个默认的图片或者保留为空，在 paint 里处理
    }
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    
    // 断开 MyObject 默认的 GameOver -> deleteLater 连接
    // 因为 Trophy 的销毁将由 GameScene 的销毁来触发，避免重复删除或时序问题
    disconnect(this, &MyObject::GameOver, this, &QGraphicsObject::deleteLater);
    this->setScale(0.5);
}

QRectF Trophy::boundingRect() const
{
    if (!pixmap.isNull()) {
        return QRectF(-pixmap.width()/2, -pixmap.height()/2, pixmap.width(), pixmap.height());
    }
    return QRectF(-50, -50, 100, 100); // 默认大小
}

void Trophy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (!pixmap.isNull()) {
        painter->drawPixmap(-pixmap.width()/2, -pixmap.height()/2, pixmap);
    } else {
        // 图片加载失败，画一个金色的奖杯替代品
        painter->setBrush(Qt::yellow);
        painter->drawEllipse(-40, -40, 80, 80);
        painter->setBrush(Qt::NoBrush);
        painter->setPen(Qt::black);
        painter->drawText(QRectF(-40, -40, 80, 80), Qt::AlignCenter, "Trophy");
    }
}

Trophy::~Trophy()
{
}

void Trophy::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (isClicked) return; // 防止重复点击

    if (event->button() == Qt::LeftButton) {
        //播放音乐
        playSoundEffect("qrc:/res/GameRes/audio/winmusic.mp3");
        isClicked = true;
        // 点击后自动匀速放大
        // 使用 Animate 类，设置一个较大的目标缩放值和较长的持续时间
        // 例如：在 5 秒内放大到 20 倍
        Animate(this).duration(AnimationType::Move,2000).move(QPointF(Coordinate().getX(4),Coordinate().getY(2)),false);
        Animate(this).duration(AnimationType::Scale, 2000).scale(2.0).finish(AnimationType::Scale, [=](){
            qDebug() << "结束";
            // 使用 QTimer::singleShot 延后触发，避免在动画回调栈中直接销毁场景导致卡死
            // 增加延时到 500ms，确保动画系统完全退出
            QTimer::singleShot(2500, this, [=](){
                emit victoryAnimationFinished();
            });
        });
        
        // 禁用再次点击，防止重复触发
        setFlag(QGraphicsItem::ItemIsSelectable, false);
        setAcceptHoverEvents(false);
    }
    MyObject::mousePressEvent(event);
}
