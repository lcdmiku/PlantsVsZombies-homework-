#include "dominator.h"
#include"gamescene.h"
#include"animate.h"
#include"sunlight.h"
#include<QDrag>
#include<QMimeData>

Dominator::Dominator():MyObject(nullptr,QString(":/res/GameRes/images/muliBoki.gif"),Type::Dominator),
    speed(200),speedRate(1.0),coordinate()
{
    dialog = new DialogBox(this);
    QVector<QString> btnStrs;
    btnStrs.push_back("hahahaha");
    dialog->setDialog("MuliMuli......",btnStrs);
}
//显示要说的话
void Dominator::setDialog(QString info,QVector<QString> btnStrs){
    dialog->setDialog(info,btnStrs);
}
//僵尸生成
void Dominator::ZombieGenerate(ZombieType zombieType,int row,int x){
    GameScene* gamescene = getGameScene();
    if(gamescene){
        gamescene->ZombieGenerate(zombieType,row,x);
    }
}
//移除僵尸
void Dominator::wipeZombie(QPointF pos,int num){
    //运动到指定位置后，删除该位置的僵尸
    GameScene* gamescene = getGameScene();
    if(gamescene){
        Animate(this).speed(AnimationType::Move,speed * speedRate)
        .shape(AnimationType::Move,QEasingCurve::InOutCubic).move(pos,false).finish(AnimationType::Move,[=,&num](){

            QList<QGraphicsItem*> collisions = collidingItems();
            foreach (QGraphicsItem* item, collisions) {
                MyObject *zombie = dynamic_cast<Zombie*>(item);
                if(zombie && !zombie->IsDead()){
                    gamescene->removeItem(zombie);
                    delete zombie;
                    if(--num)break;
                }
            }
        });
    }
}
//种植植物
void Dominator::plant(enum PlantType plantype,int r,int c){
    GameScene* gamescene = getGameScene();
    if(gamescene){
        PlantArea* area = gamescene->getPlantArea(r,c);
        if(area){
            area->plant(plantype);
        }
    }
}
//铲除植物
void Dominator::uproot(int r,int c){
    GameScene* gamescene = getGameScene();
    if(gamescene){
        PlantArea* area = gamescene->getPlantArea(r,c);
        if(area){
            area->removePlant();
        }
    }
}
//偷取阳光
void Dominator::stealSunlight(int num){
    GameScene* gamescene = getGameScene();
    if(gamescene){
        Shop* shop = gamescene->getShop();
        if(shop){
            shop->reduceSunlight(num);
        }
    }
}
void Dominator::stealSunlight(int num,int eachVal){
    GameScene* gamescene = getGameScene();
    if(gamescene){
        Shop* shop = gamescene->getShop();
        if(shop){
            QPointF start = shop->pos();
            QPointF end = pos();

        }
    }
}
//给予阳光
void Dominator::giveSunlight(int num){
    GameScene* gamescene = getGameScene();
    if(gamescene){
        Shop* shop = gamescene->getShop();
        if(shop){
            shop->increaseSunlight(num);
        }
    }
}
void Dominator::giveSunlight(int num,int eachVal){
    GameScene* gamescene = getGameScene();
    if(gamescene){
        Shop* shop = gamescene->getShop();
        if(shop){
            QPointF start = pos();
            for (int var = 0; var < num; ++var) {
                SunLight * sunlight = new SunLight(eachVal);
                gamescene->addItem(sunlight);
            }
        }
    }
}
void Dominator::setCurrentGif(){

}
//用户行为模拟
//模拟拖拽
void Dominator::simulateDrag(QPointF pos,QPointF toPos,QPixmap pixmap){
    QDrag *drag = new QDrag(scene()->parent());
    QMimeData *mimeData = new QMimeData;

    // 仅传递必要的植物类型数据
    mimeData->setData("shovel/remove", QByteArray::number(static_cast<bool>(true)));

    drag->setMimeData(mimeData);

    // 执行拖拽
    drag->setHotSpot(QPoint(30,30));
    drag->exec(Qt::CopyAction);

}
