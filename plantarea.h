#ifndef PLANTAREA_H
#define PLANTAREA_H

#include<QMovie>
#include <QGraphicsObject>
#include<QPainter>
#include <QRandomGenerator>
#include<QGraphicsSimpleTextItem>
#include<QPropertyAnimation>
#include<QTimer>
#include<QGraphicsSceneMouseEvent>
#include"card.h"
#include"peashooter.h"
#include"sunflower.h"
#include"wallnut.h"
#include"cherrybomb.h"
#include"potatomine.h"
#include"snowpeashooter.h"
#include<QGraphicsSceneDragDropEvent>
#include<QGraphicsSceneMouseEvent>

enum class LandType{
    None,
    Nomal,
    DryLand,
    Water
};

class PlantArea:public MyObject
{
    Q_OBJECT
    int width;
    int height;
    int row;
    int col;
    bool isEmpty;
    bool plantable;

    QGraphicsPixmapItem *bg;
    Plant *Myplant;



    //
    void setEmpty(bool isEmpty){this->isEmpty = isEmpty;}

    void dealLandType();
public:

    explicit PlantArea(int row,int col,enum LandType landType = LandType::Nomal);
    // 必须实现的虚函数
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    //static int sunlightGenerate();
    int w(){return width;}
    int h(){return height;}
    int r(){return row;}
    int c(){return col;}
    //
    bool checkEmpty(){return isEmpty;}
    bool removePlant();
    //设置该地方是否能种,以及覆盖物
    void setPlantable(bool plantable,QString coverage);
    //向外界提供该地植物指针
    Plant* getPlant();
    //获取是否ke种植植物
    bool getIsPlantable(){return plantable && !isEmpty;}
    ~PlantArea();

signals:
    void sunlightProduced(SunLight *sunlight);
    void needToPlant(int cost,enum PlantType plantType);
    void GameOver();

    // protected:
    //     void advance(int phase) override;
protected:
    const enum LandType landType;


    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

    //
public:
    bool plant(enum PlantType plantType);//种植植物的接口
};

#endif // PLANTAREA_H
