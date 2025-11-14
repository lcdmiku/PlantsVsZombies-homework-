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
    explicit SunLight(int sunlightVal);//可控制生成阳光数的
    // 必须实现的虚函数
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    //static int sunlightGenerate();
    ~SunLight();
    void beCollected();//共外界使用，使用后生成的阳光被收集,有可能采集负数，表示阳光被掠夺

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
