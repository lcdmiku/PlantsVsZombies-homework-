#ifndef ANIMATE_H
#define ANIMATE_H

#include <QWidget>
#include"myobj.h"
#include"gamescene.h"
#include<QEasingCurve>
#include<QPropertyAnimation>
#include<QParallelAnimationGroup>
#include<QSequentialAnimationGroup>


enum class AnimationType{
    Move,
    Scale,
    Opacity
};

class Animate
{
    QPropertyAnimation *anim_m;
    QPropertyAnimation* anim_s;
    QPropertyAnimation* anim_o;
    //
    int type;
    int m_duration_m;
    int m_duration_s;
    int m_duration_o;
    qreal m_speed_m;
    qreal m_speed_s;
    qreal m_speed_o;
    QPointF dir;
    QPointF toPos;
    qreal toScale, toOpacity;
    QEasingCurve::Type m_shape_m;
    QEasingCurve::Type m_shape_s;
    QEasingCurve::Type m_shape_o;
    //
    MyObject* target;
    QGraphicsScene *scene;
    //使用QHash管理target的动画实例
    static QHash<MyObject*,QPropertyAnimation*> moveAnim;
    static QHash<MyObject*,QPropertyAnimation*> scaleAnim;
    static QHash<MyObject*,QPropertyAnimation*> opacityAnim;
public:
    Animate(MyObject* target,GameScene* scene);
    ~Animate();
    Animate &move(QPointF toPos,bool asDir=true);
    Animate &scale(qreal toScale);
    Animate &fade(qreal toOpacity);

    Animate &replace();
    Animate &shape(enum AnimationType animType,QEasingCurve::Type shape);
    Animate &speed(enum AnimationType animType,qreal speed);
    Animate &duration(enum AnimationType animType,int duration);

    //需要知道是哪种动画停止
    Animate &finish(enum AnimationType animType,std::function<void(void)> functor);
    Animate &finish(enum AnimationType animType,std::function<void(bool)> functor = [](bool) {});


};

#endif // ANIMATE_H
