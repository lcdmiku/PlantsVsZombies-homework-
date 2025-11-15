#include "sunlight.h"
#include<QGraphicsScene>


SunLight::SunLight()
    : QGraphicsObject(nullptr),movie(nullptr),sunlightTroughPos(290,0),sunlightValue(0), // 初始化两个基类
    clickable(true)
{
    //
    timer = new QTimer();
    connect(timer,&QTimer::timeout,this,&QObject::deleteLater);
    timer->start(20000);//1 min
    //
    movie = new QMovie(":/res/GameRes/images/Sun.gif",QByteArray(),this);
    if(movie->isValid()){
        movie->setCacheMode(QMovie::CacheAll);
        connect(movie,&QMovie::frameChanged,this,&SunLight::frameChanged);

        //big or small sunlight
        int gen = QRandomGenerator::global()->bounded(1,3);
        sunlightValue = gen * 50;
        movie->setScaledSize(QSize(50*(0.5*gen+0.5),50*(0.5*gen+0.5)));

        movie->start();
    }
    // connect(this,&SunLight::sunlightCollected,scene()->parent(),)
    // 构造函数实现
}
SunLight::SunLight(int sunlightVal)
    : QGraphicsObject(nullptr),movie(nullptr),sunlightTroughPos(290,0),sunlightValue(sunlightVal) // 初始化两个基类
{
    //
    timer = new QTimer();
    connect(timer,&QTimer::timeout,this,&QObject::deleteLater);
    timer->start(20000);//1 min
    //
    movie = new QMovie(":/res/GameRes/images/Sun.gif",QByteArray(),this);
    if(movie->isValid()){
        movie->setCacheMode(QMovie::CacheAll);
        connect(movie,&QMovie::frameChanged,this,&SunLight::frameChanged);

        int gen = abs(sunlightValue) / 50;
        movie->setScaledSize(QSize(50*(0.5*gen+0.5),50*(0.5*gen+0.5)));

        movie->start();
    }
}
QRectF SunLight::boundingRect() const{
    if(movie && movie->state() == QMovie::Running){
        return QRectF(0,0,movie->currentPixmap().width(),movie->currentPixmap().height());
    }
    else return QRectF(0,0,50,50);
}

void SunLight::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(movie && movie->state() == QMovie::Running){
        painter->drawPixmap(0,0,movie->currentPixmap());
    }
    else painter->drawPixmap(0,0,QPixmap(50,50));
}

void SunLight::frameChanged(int frameNumber){
    Q_UNUSED(frameNumber);
    update(); // 通知场景重绘此图形项
}
//共外界使用，使用后生成的阳光被收集
void SunLight::beCollected(){
    isCollected = true;

    QPropertyAnimation *animation = new QPropertyAnimation(this,"pos");
    animation->setDuration(1000);
    animation->setStartValue(pos());
    animation->setEndValue(sunlightTroughPos);

    //传递消息给shop
    emit sunlightCollected(sunlightValue);
    //结束后delete sunlight
    connect(animation,&QPropertyAnimation::finished,this,&QObject::deleteLater);

    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void SunLight::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(clickable)
    beCollected();

}
void SunLight::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {}
void SunLight::hoverEnterEvent(QGraphicsSceneHoverEvent *event){}
void SunLight::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {}

SunLight::~SunLight(){
    disconnect(this);
    if(scene())scene()->removeItem(this);//
    delete timer;
}
