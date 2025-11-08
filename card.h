#ifndef CARD_H
#define CARD_H
#include<QMovie>
#include <QGraphicsObject>
#include<QPainter>
#include <QRandomGenerator>
#include<QGraphicsSimpleTextItem>
#include<QPropertyAnimation>
#include<QTimer>
#include<QGraphicsSceneMouseEvent>

//:/res/GameRes/images/Card.png
enum class PlantType{
    None,
    PEASHOOTER,
    SUNLIGHT,
    WALLNUT,
    CHEERYBOMB,
    POTATOMINE,
    SNOWPEASHOOTER
};

class Card: public QGraphicsObject
{
    Q_OBJECT
    QGraphicsPixmapItem *pixmap;
    QGraphicsPixmapItem *plantpix;
    QString plantPath;
    QGraphicsSimpleTextItem *sunlightText;
    enum PlantType plantType;
    QPointF m_dragStartPosition;
    int sunlightCost;
    //冷却
    // 注册cooldownProgress属性，用于动画驱动
    Q_PROPERTY(qreal cooldownProgress READ cooldownProgress WRITE setCooldownProgress)
    int cooldownTime;//冷却时间
    qreal m_cooldownProgress;//冷却状态
    bool isReady;//是否冷却完毕
public:
    explicit Card(QGraphicsObject *parent,QString plantpath);
    // 必须实现的虚函数
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    //得到卡片种类
    enum PlantType getPlantType(){return plantType; }
    QString getPlantPath(){return plantPath;}

    //冷却
    bool IsReady(){return isReady;}
    // getter：返回当前冷却进度
    qreal cooldownProgress() const { return m_cooldownProgress; }
    // setter：更新冷却进度并触发重绘
    void setCooldownProgress(qreal progress){
        // 限制进度在0.0~1.0之间
        m_cooldownProgress = qBound(0.0, progress, 1.0);
        update(); // 触发paint()重绘，更新进度条显示
    }
    void CoolDown();
    ~Card();

signals:
    void cardPress(Card *card);


    // protected:
    //     void advance(int phase) override;
protected:
    // 鼠标事件处理函数
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    //

private slots:

};

#endif // CARD_H
