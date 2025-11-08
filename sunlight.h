#ifndef SUNLIGHT_H
#define SUNLIGHT_H

#include<QMovie>
#include <QGraphicsObject>
#include<QPainter>
#include <QRandomGenerator>
#include<QPropertyAnimation>
#include<QTimer>


class SunLight : public QGraphicsObject
{
    Q_OBJECT
    QMovie *movie;
    QTimer *timer;//阳光存在时间
    bool isCollected;
    int sunlightValue;
    QPointF sunlightTroughPos;
public:
    explicit SunLight();
    // 必须实现的虚函数
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    //static int sunlightGenerate();
    ~SunLight();

signals:
    void sunlightCollected(int sunlightValue);


// protected:
//     void advance(int phase) override;
protected:
    // 鼠标事件处理函数
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    //

private slots:
    void frameChanged(int frameNumber);
};

#endif // SUNLIGHT_H
