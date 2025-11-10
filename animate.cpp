#include "animate.h"

QHash<MyObject*,QPropertyAnimation*> Animate::moveAnim;
QHash<MyObject*,QPropertyAnimation*> Animate::scaleAnim;
QHash<MyObject*,QPropertyAnimation*> Animate::opacityAnim;

Animate::Animate(MyObject* target,GameScene* scene):
    type(0),
    m_duration_m(0),m_duration_s(0),m_duration_o(0)
    , m_speed_m(5),m_speed_s(5),m_speed_o(5),
    toPos(), toScale(0), toOpacity(0),
    m_shape_m(QEasingCurve::Linear),
    m_shape_s(QEasingCurve::Linear),
    m_shape_o(QEasingCurve::Linear),
    target(target), scene(scene)
{//
    if(moveAnim.contains(target))anim_m = moveAnim[target];
    else{
        anim_m = new QPropertyAnimation(target,"pos",target);
        moveAnim.insert(target,anim_m);
    }
    //
    if(scaleAnim.contains(target))anim_s = scaleAnim[target];
    else{
        anim_s = new QPropertyAnimation(target,"scale",target);
        scaleAnim.insert(target,anim_s);
    }
    //
    if(opacityAnim.contains(target))anim_o = opacityAnim[target];
    else{
        anim_o = new QPropertyAnimation(target,"opacity",target);
        opacityAnim.insert(target,anim_o);
    }
}
Animate::~Animate(){
    //内存管理留给QT父指针制度
    // delete anim_m;
    // moveAnim.remove(target);
    // delete anim_s;
    // scaleAnim.remove(target);
    // delete anim_o;
    // opacityAnim.remove(target);
}
Animate& Animate::move(QPointF toPos,bool asDir){
    //暂停动画，防止错乱
    if(anim_m->state() == QAbstractAnimation::Running){
        anim_m->pause();
    }
    //重新设置各项属性
    anim_m->setStartValue(target->pos());//将当前位置作为动画开始
    //分两种情况，toPos是dir或者是dest
    if(asDir){//作为方向的情况
        this->toPos = target->pos() + toPos;
    }
    else{//作为目的地的情况
        this->toPos = toPos;
    }
    anim_m->setEndValue(this->toPos);
    //
    if(m_duration_m<=0){//可以用速度计算出来
        QPointF posVec = this->toPos - target->pos();
        m_duration_m = qRound(qSqrt(QPointF::dotProduct(posVec, posVec)) / m_speed_m);
    }

    anim_m->setDuration(m_duration_m);

    anim_m->start();
    return *this;
}
Animate& Animate::scale(qreal toScale){
    //暂停动画，防止错乱
    if(anim_s->state() == QAbstractAnimation::Running){
        anim_s->pause();
    }
    //重新设置各项属性
    anim_s->setStartValue(target->scale());//将当前位置作为动画开始
    this->toScale = toScale;

    anim_s->setEndValue(this->toScale);

    anim_s->start();
    return *this;
}
Animate& Animate::fade(qreal toOpacity){
    //暂停动画，防止错乱
    if(anim_o->state() == QAbstractAnimation::Running){
        anim_o->pause();
    }
    //重新设置各项属性
    this->toOpacity = toOpacity;
    anim_o->setStartValue(target->opacity());//将当前位置作为动画开始

    anim_o->setEndValue(this->toOpacity);

    anim_o->start();
    return *this;
}


Animate& Animate::replace(){
    return *this;
}
Animate& Animate::shape(enum AnimationType animType,QEasingCurve::Type shape){
    switch (animType) {
    case AnimationType::Move:
        m_shape_m = shape;
        break;
    case AnimationType::Scale:
        m_shape_s = shape;
        break;
    case AnimationType::Opacity:
        m_shape_o = shape;
        break;
    default:
        break;
    }
    return *this;
}
Animate& Animate::speed(enum AnimationType animType,qreal speed){
    switch (animType) {
    case AnimationType::Move:
        m_speed_m = speed;
        break;
    case AnimationType::Scale:
        m_speed_s = speed;
        break;
    case AnimationType::Opacity:
        m_speed_o = speed;
        break;
    default:
        break;
    }
    return *this;
}
Animate& Animate::duration(enum AnimationType animType,int duration){

    switch (animType) {
    case AnimationType::Move:
        m_duration_m = duration;
        break;
    case AnimationType::Scale:
        m_duration_m = duration;
        break;
    case AnimationType::Opacity:
        m_duration_m = duration;
        break;
    default:
        break;
    }
    return *this;
}

Animate& Animate::finish(enum AnimationType animType,std::function<void(void)> functor){
    switch (animType) {
    case AnimationType::Move:
        if(target->pos() == toPos){functor();}
        break;
    case AnimationType::Scale:
        if(target->scale() == toScale){functor();}
        break;
    case AnimationType::Opacity:
        if(target->opacity() == toOpacity){functor();}
        break;
    default:
        break;
    }
    return *this;
}
Animate& Animate::finish(enum AnimationType animType,std::function<void(bool)>){
    return *this;
}

