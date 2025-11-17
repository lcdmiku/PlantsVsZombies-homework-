#include "animate.h"

QHash<MyObject*,QPropertyAnimation*> Animate::moveAnim;
QHash<MyObject*,QPropertyAnimation*> Animate::scaleAnim;
QHash<MyObject*,QPropertyAnimation*> Animate::opacityAnim;
QHash<MyObject*,Animate::Animation*> Animate::animation;

//注意：：每次调用都意味着要给出完整动画，不能想当然与之前的动画关联（Animate使用指针版除外）
Animate::Animate(MyObject* target):
    target(target)
{//
    if(!moveAnim.contains(target) || !scaleAnim.contains(target) || !opacityAnim.contains(target)){
        //仅在第一次target建立动画时绑定，后续无法绑定,管理哈希表，防止野指针
        QObject::connect(target,
                         &MyObject::needToDel,
                         [=](){
                             moveAnim.remove(target);
                             scaleAnim.remove(target);
                             opacityAnim.remove(target);
                             animation.remove(target);
                         });

    }
    //管理哈希表，防止野指针
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
    if(animation.contains(target))anim = animation[target];
    else{
        anim = new Animation();
        animation.insert(target,anim);

    }
    // QObject::connect(anim_m,&QPropertyAnimation::valueChanged,[=](){
    //     cnt_m = (cnt_m+1) % interval_m;
    //     if(cnt_m==0)emit m_timeout();
    // });

    //

}
QPropertyAnimation* Animate::getAnim(enum AnimationType animType){
    switch (animType) {
    case AnimationType::Move:
        return anim_m;
    case AnimationType::Scale:
        return anim_s;
    case AnimationType::Opacity:
        return anim_o;
    default:
        return nullptr;
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
    AnimateParam& param = anim->params[AnimationType::Move];
    //暂停动画，防止错乱
    if(anim_m->state() == QAbstractAnimation::Running){
        anim_m->stop();
    }
    //重新设置各项属性
    QPointF currPos = target->pos();

    //分两种情况，toPos是dir或者是dest
    param.toPos = asDir ? currPos + toPos : toPos;

    //
    //可以用速度计算出来
    if(param.duration<=0)
    {
        QPointF posVec = param.toPos - currPos;
        param.duration = std::max(10,qRound(qSqrt(QPointF::dotProduct(posVec, posVec)) / param.speed) * 1000);// p/s
    }

    anim_m->setStartValue(currPos);
    anim_m->setEndValue(param.toPos);
    anim_m->setDuration(param.duration);
    anim_m->setEasingCurve(param.shape);

    if (anim_m->state() == QAbstractAnimation::Paused) {
        // qDebug()<<"pause resume"<<currPos;
        anim_m->resume();
    } else if(anim_m->state() == QAbstractAnimation::Stopped) {
        // qDebug()<<"stop resume"<<currPos;
        anim_m->start();
    }
    param.duration = 0;//确保下次用speed计算
    return *this;
}
Animate& Animate::scale(qreal toScale){
    AnimateParam& param = anim->params[AnimationType::Scale];
    //暂停动画，防止错乱
    if(anim_s->state() == QAbstractAnimation::Running){
        anim_s->pause();
    }
    //重新设置各项属性
    qreal currScale = target->scale();
    param.toScale = toScale;

    if (anim_s->state() == QAbstractAnimation::Running) {
        //动画正在运行,直接更新属性，不停止
        anim_s->setEndValue(param.toScale);
        anim_s->setDuration(param.duration);
        anim_s->setEasingCurve(param.shape);
    } else  {
        //从头开始
        anim_s->setStartValue(currScale);
        anim_s->setEndValue(param.toScale);
        anim_s->setDuration(param.duration);
        anim_s->setEasingCurve(param.shape);

        anim_s->start();
    }
    return *this;
}
Animate& Animate::fade(qreal toOpacity){
    AnimateParam& param = anim->params[AnimationType::Opacity];
    //暂停动画，防止错乱
    if(anim_o->state() == QAbstractAnimation::Running){
        anim_o->pause();
    }
    //重新设置各项属性
    param.toOpacity = toOpacity;
    qreal currOpacity = target->opacity();

    if (anim_o->state() == QAbstractAnimation::Running) {
        //动画正在运行,直接更新属性，不停止
        anim_o->setEndValue(param.toOpacity);
        anim_o->setDuration(param.duration);
        anim_o->setEasingCurve(param.shape);
    } else  {
        //从头开始
        anim_o->setStartValue(currOpacity);
        anim_o->setEndValue(param.toOpacity);
        anim_o->setDuration(param.duration);
        anim_o->setEasingCurve(param.shape);

        anim_o->start();
    }
    return *this;
}


Animate& Animate::replace(){
    return *this;
}
Animate& Animate::shape(enum AnimationType animType,QEasingCurve::Type shape){
    if(animType != AnimationType::All)
    {
        AnimateParam& param = anim->params[animType];
        param.shape = shape;
    }
    return *this;
}

Animate& Animate::speed(enum AnimationType animType,qreal speed){
    if(animType != AnimationType::All)
    {
        AnimateParam& param = anim->params[animType];
        param.speed = speed;
    }
    return *this;
}
Animate& Animate::duration(enum AnimationType animType,int duration){

    if(animType != AnimationType::All)
    {
        AnimateParam& param = anim->params[animType];
        param.duration = duration;
    }
    return *this;
}

Animate& Animate::finish(enum AnimationType animType,std::function<void(bool)> func){
    AnimateParam& param = anim->params[animType];
    param.finished = func;
    QPropertyAnimation* animInst = getAnim(animType);
    if (animInst) {
        QObject::disconnect(animInst, &QPropertyAnimation::finished, nullptr, nullptr);
        QObject::connect(animInst, &QPropertyAnimation::finished, animInst, [this, animType, func,animInst]() {
            QObject::disconnect(animInst, &QPropertyAnimation::finished, nullptr, nullptr);//一次性
            func(true);
        });
    }

    return *this;
}
Animate& Animate::finish(enum AnimationType animType,std::function<void(void)> func){
    return finish(animType, [func](bool) {
        if (func) {
            func();
        }
    });
}

Animate& Animate::pause(enum AnimationType animType){
    //停止动画
    switch (animType) {

    case AnimationType::Move:
        if(anim_m->state() == QAbstractAnimation::Running)
        {
            anim_m->pause();
            qDebug()<<"pause22"<<anim_m->state();
        }
        break;
    case AnimationType::Scale:
        if(anim_s->state() == QAbstractAnimation::Running)
        anim_s->pause();
        break;
    case AnimationType::Opacity:
        if(anim_o->state() == QAbstractAnimation::Running)
        anim_o->pause();
        break;
    case AnimationType::All:
        this->pause(AnimationType::Move);
        this->pause(AnimationType::Scale);
        this->pause(AnimationType::Opacity);
        break;
    default:
        break;
    }
    return *this;
}
Animate& Animate::stop(enum AnimationType animType){
    //停止动画
    switch (animType) {
    case AnimationType::Move:
        if(anim_m->state() == QAbstractAnimation::Running)
            anim_m->stop();
        break;
    case AnimationType::Scale:
        if(anim_s->state() == QAbstractAnimation::Running)
            anim_s->stop();
        break;
    case AnimationType::Opacity:
        if(anim_o->state() == QAbstractAnimation::Running)
            anim_o->stop();
        break;
    case AnimationType::All:
        this->stop(AnimationType::Move);
        this->stop(AnimationType::Scale);
        this->stop(AnimationType::Opacity);
        break;
    default:
        break;
    }
    return *this;
}
Animate& Animate::resume(enum AnimationType animType){
    //停止动画
    switch (animType) {
    case AnimationType::Move:
        qDebug()<<"resume"<<anim_s->state();

        if(anim_s->state() == QAbstractAnimation::Paused)
        {
            anim_s->resume();
        }
        if(anim_s->state()==QAbstractAnimation::Stopped)
            anim_s->start();
        break;
    case AnimationType::Scale:
        if(anim_s->state() == QAbstractAnimation::Paused)
            anim_s->resume();
        if(anim_s->state()==QAbstractAnimation::Stopped)
            anim_s->start();
        break;
    case AnimationType::Opacity:
        if(anim_o->state() == QAbstractAnimation::Paused)
            anim_o->resume();
        if(anim_o->state()==QAbstractAnimation::Stopped)
            anim_o->start();
        break;
    case AnimationType::All:
        this->resume(AnimationType::Move);
        this->resume(AnimationType::Scale);
        this->resume(AnimationType::Opacity);
    default:
        break;
    }
    return *this;
}
Animate& Animate::trigger(enum AnimationType animType,std::function<void(void)> functor){

    if(target){
        functor();
    }
    return *this;
}
