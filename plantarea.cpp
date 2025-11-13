#include "plantarea.h"
#include<QDebug>
#include<QMimeData>
#include<iostream>

PlantArea::PlantArea(int row,int col,enum LandType landType)
    :MyObject(nullptr),
    width(81),height(94),isEmpty(true),landType(landType),bg(nullptr),Myplant(nullptr),row(row),col(col)
{
   setAcceptHoverEvents(true); // 关键：允许接收悬停事件
    setAcceptDrops(true); // 启用拖放
    dealLandType();
    connect(this,&PlantArea::GameOver,this,&QGraphicsObject::deleteLater);

}
// 必须实现的虚函数
QRectF PlantArea::boundingRect() const {
    return QRectF(0,0,width,height);
}
void PlantArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if(bg){
        painter->drawPixmap(0,0,bg->pixmap());
    }
    painter->setPen(Qt::white);
    painter->drawRect(this->boundingRect());
 }
//static int sunlightGenerate();
PlantArea::~PlantArea(){
    disconnect(this);
}

void PlantArea::dealLandType(){
    if(landType == LandType::Nomal || landType == LandType::None)return;
    if(landType == LandType::DryLand){
        bg = new QGraphicsPixmapItem(QPixmap(":/res/GameRes/images/unplantableground.jpg"),this);
        update();
    }

}



void PlantArea::plant(enum PlantType plantType){
    Plant *newPlant = nullptr;

    if(checkEmpty() && landType != LandType::DryLand){
        switch (plantType) {
        case PlantType::PEASHOOTER:
        {
            qDebug()<<"plant pea";
            newPlant = new Peashooter(this);
            break;
        }
        case PlantType::SUNLIGHT:
        {
            qDebug()<<"plant sun";

            newPlant = new SunFlower(this);
            connect(qobject_cast<SunFlower*>(newPlant),&SunFlower::sunlightProduce,this,&PlantArea::sunlightProduced);
            break;
        }
        case PlantType::WALLNUT:
        {
            qDebug()<<"plant nut";

            newPlant = new WallNut(this);
            break;
        }
        case PlantType::CHEERYBOMB:
        {
            qDebug()<<"plant bomb";

            newPlant = new CherryBomb(this);
            break;
        }
        case PlantType::POTATOMINE:
        {
            qDebug()<<"plant potato";

            newPlant = new PotatoMine(this);
            break;
        }
        case PlantType::SNOWPEASHOOTER:
        {
            qDebug()<<"plant snow";

            newPlant = new SnowPeashooter(this);
            break;
        }
        default:
            break;
        }
        connect(newPlant,&Plant::dead,this,[=](){
            //当植物消亡时设置为空
            Myplant = nullptr;
            setEmpty(true);
        });
        newPlant->setPos(width/2,height/2);
        connect(this,&PlantArea::GameOver,newPlant,&MyObject::GameOver);

        Myplant = newPlant;
        setEmpty(false);
    }
}


// 拖拽事件
void PlantArea::dropEvent(QGraphicsSceneDragDropEvent *event) {
//检测数据类型
if(event->mimeData()->hasFormat("plant/type") && event->mimeData()->hasFormat("plant/cost")){
    event->setAccepted(true);
    int plantInt = event->mimeData()->data("plant/type").toInt();
    int cost = event->mimeData()->data("plant/cost").toInt();

    enum PlantType plantType = static_cast<enum PlantType>(plantInt);
    std::cout<<plantInt<<std::endl;
    if(checkEmpty() && landType != LandType::DryLand )emit needToPlant(cost,plantType);
}
//shovel/remove
if(event->mimeData()->hasFormat("shovel/remove") && Myplant){
    event->setAccepted(true);
    Myplant->DealDead();
    Myplant = nullptr;
    setEmpty(true);
}

}
void PlantArea::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
    //检测数据类型
    if(event->mimeData()->hasFormat("plant/type")){
        event->setAccepted(true);

        // qDebug() << "Drag enter";
    }
}

void PlantArea::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {
    //检测数据类型
    if(event->mimeData()->hasFormat("plant/type")){
        event->setAccepted(true);


    }
}
void PlantArea::dragLeaveEvent(QGraphicsSceneDragDropEvent *event) {
    //检测数据类型
    if(event->mimeData()->hasFormat("plant/type")){
        event->setAccepted(true);

        // qDebug() << "Drag leave";
    }
}




