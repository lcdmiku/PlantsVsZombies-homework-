#include "myobj.h"
#include"animate.h"
#include"gamescene.h"
#include<QDebug>



MyObject::MyObject(QGraphicsObject *parent,QString objPath,const enum Type type)
    : QGraphicsObject(parent),movie(nullptr),type(type),CurrentGif(objPath),isDead(false)
{
    //得到scene
    gameScene = dynamic_cast<GameScene*>(this->scene());
    if(!gameScene){
        // qDebug()<<"gameScene 转化失败，对象创建失败"<<objPath;
    }//一般类实例初始化时还没加入场景，所以不能在这时候初始化gamescene,另外写函数
    //启动计时器并连接
    timer = new QTimer();
    connect(timer,&QTimer::timeout,this,[=](){
        //如果没死
        if(!isDead)CheckMeet();
    });//时刻检查碰撞
    connect(timer,&QTimer::timeout,this,[=](){
        setCurrentGif();
        if(!isDead)
        ToCurrentGif();
    });//时刻检查此时情况的gif并更新
    connect(this,&MyObject::GameOver,this,&QGraphicsObject::deleteLater);//游戏结束要delete掉

    timer->start(100);
    //启动movie
    if(!objPath.isEmpty())
    {
        movie = new QMovie(objPath,QByteArray(),this);
        if(movie->isValid()){
            movie->setCacheMode(QMovie::CacheAll);
            connect(movie,&QMovie::frameChanged,this,&MyObject::frameChanged);
            connect(movie,&QMovie::finished,this,&MyObject::movieFinished);
            movie->start();
        }
    }
    // 构造函数实现

}
GameScene* MyObject::getGameScene(){
    gameScene = dynamic_cast<GameScene*>(this->scene());
    if(!gameScene){
        qDebug()<<"gameScene 转化失败，无法获得游戏场景数据";
    }//一般类实例初始化时还没加入场景，所以不能在这时候初始化gamescene,另外写函数
    return gameScene;
}
QRectF MyObject::boundingRect() const{
    if(movie){
        int w = movie->currentPixmap().width();
        int h = movie->currentPixmap().height();

        return QRectF(-w/2,-h/2,w,h);
    }
    else {
        return QRectF(25,25,50,50);
    }
}

void MyObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //测试代码
    painter->setPen(Qt::white);
    painter->drawRect(boundingRect());
    //绘制碰撞区域（红色边框）
    painter->setPen(Qt::red);
    painter->drawPath(shape());

    int w = 50;
    int h = 50;
    if(movie){
        w = movie->currentPixmap().width();
        h = movie->currentPixmap().height();
        painter->drawPixmap(-w/2,-h/2,movie->currentPixmap());
    }
    else painter->drawPixmap(-w/2,-h/2,QPixmap(50,50));
}

void MyObject::frameChanged(int frameNumber){
    Q_UNUSED(frameNumber);
    update(); // 通知场景重绘此图形项
}

void MyObject::CheckMeet(){
    //检测与Enemy类的碰撞
    QList<QGraphicsItem*> collisions = collidingItems();
    foreach (QGraphicsItem* item, collisions) {
        MyObject *obj = dynamic_cast<MyObject*>(item);
        if(obj && obj->type != this->type && !obj->isDead){
            emit MyObject::Meet(obj);
        }
    }
}

void MyObject::changeGif(QString newPath){

    if(movie && movie->fileName() != newPath)//仅当俩者不同时变化，提高效率；
    {
        movie->stop();                // 停止当前动画
        movie->setFileName(newPath);  // 设置新路径

        if (movie->isValid()) {       // 检查文件是否有效
            movie->start();           // 启动新动画
        } else {
            qDebug() << "Failed to load GIF:" << newPath;
        }
    }
}

void MyObject::setCurrentGif(){

}

void MyObject::ToCurrentGif(){

     this->changeGif(CurrentGif);
}

MyObject::~MyObject(){
    emit needToDel();
    disconnect(this);

    delete timer;
}
void MyObject::GamePause(){
    if(timer->isActive()){
        timer->stop();
    }
    Animate(this).pause(AnimationType::All);
    qDebug()<<"pause1";
}
void MyObject::GameContinue(){
    if(!timer->isActive()){
        timer->start();
    }
    Animate(this).resume(AnimationType::All);
    qDebug()<<"continue";
}
