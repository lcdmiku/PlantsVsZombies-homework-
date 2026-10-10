#ifndef ANIMATE_H
#define ANIMATE_H

#include <QWidget>
#include"gamescene.h"
#include<QEasingCurve>
#include<QPropertyAnimation>
#include<QParallelAnimationGroup>
#include<QSequentialAnimationGroup>

enum class AnimationType{
    Move,
    Scale,
    Opacity,
    All
};


class Animate
{

    struct AnimateParam {
        int duration;
        qreal speed; // if duration == 0, speed is used
        QPointF toPos;
        qreal toScale, toOpacity;
        QEasingCurve shape;
        std::function<void(bool)> finished;
        AnimateParam():duration(0),speed(20),
            toPos(),toScale(0),toOpacity(0),
            shape(QEasingCurve::Linear),
            finished([](bool) {}){}
    };
    struct Animation {
        QMap<enum AnimationType,AnimateParam> params;
        Animation():params(){
            params[AnimationType::Move] = AnimateParam();
            params[AnimationType::Opacity] = AnimateParam();
            params[AnimationType::Scale] = AnimateParam();
        }
    };

    QPropertyAnimation *anim_m;
    QPropertyAnimation* anim_s;
    QPropertyAnimation* anim_o;
    //
    QPropertyAnimation* getAnim(enum AnimationType animType);
    //
    Animation *anim;
    //
    MyObject* target;

    //使用QHash管理target的动画实例
    static QHash<MyObject*,QPropertyAnimation*> moveAnim;
    static QHash<MyObject*,QPropertyAnimation*> scaleAnim;
    static QHash<MyObject*,QPropertyAnimation*> opacityAnim;
    static QHash<MyObject*,Animation*> animation;
public:
    Animate(MyObject* target);
    ~Animate();
    Animate &move(QPointF toPos,bool asDir=true);
    Animate &scale(qreal toScale);
    Animate &fade(qreal toOpacity);

    Animate &replace();
    Animate &shape(enum AnimationType animType,QEasingCurve::Type shape);
    Animate &speed(enum AnimationType animType,qreal speed);
    Animate &duration(enum AnimationType animType,int duration);

    //需要知道是哪种动画停止
    Animate &finish(enum AnimationType animType,std::function<void(void)> func);
    Animate &finish(enum AnimationType animType,std::function<void(bool)> func = [](bool) {});

    Animate &pause(enum AnimationType animType);//暂停动画
    Animate &resume(enum AnimationType animType);//继续动画
    Animate &stop(enum AnimationType animType);//停zi动画

    //连接target会在动画过程中触发的函数
    Animate &trigger(enum AnimationType animType,std::function<void(void)> functor);

};

#endif // ANIMATE_H
