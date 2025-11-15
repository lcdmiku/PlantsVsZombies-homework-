#include "dominator.h"
#include"gamescene.h"
#include"animate.h"
#include"sunlight.h"
#include<QDrag>
#include<QMimeData>

Dominator::Dominator():MyObject(nullptr,QString(":/res/GameRes/images/muliBoki.gif"),Type::Dominator),
    speed(200),speedRate(1.0),coordinate()
{
    dialog = new DialogBox(this);//dialg 随dominator添加到场景中
    QVector<QString> btnStrs;
    QVector<int> btnIds;
    btnStrs.push_back("haha");
    btnStrs.push_back("111");
    btnIds.push_back(1);
    btnIds.push_back(2);
    dialog->setDialog("MuliMuli......",btnStrs,btnIds);
    connect(dialog,&DialogBox::branchTriggered,this,[=](int id){
        if(id==1){
            stealSunlight(10,50);
        }
        if(id==2)giveSunlight(10,50);
        dialog->hide();
    });
}
//显示要说的话
void Dominator::setDialog(QString info,QVector<QString> btnStrs,QVector<int> btnIds){
    dialog->setDialog(info,btnStrs,btnIds);
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
            QPointF end = pos();
            for (int var = 0; var < num; ++var) {
                QPointF start = shop->pos() + QPointF(var,var);//（290，0）
                SunLight * sunlight = new SunLight(-eachVal);//设置为负数，表示掠夺
                sunlight->setClickable(false);//设置为不可点击
                gamescene->addItem(sunlight);
                sunlight->setPos(start);
                sunlight->setsunlightTroughPos(end);
                QTimer::singleShot(1000 + var * var*10,sunlight,[=](){//使用var * var*10控制动画效果
                    sunlight->beCollected();
                    shop->reduceSunlight(eachVal);//shop逻辑
                });//1s后被全部统一收集
            }
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

            for (int var = 0; var < num; ++var) {
                QPointF start = pos() + QPointF(var,var);
                SunLight * sunlight = new SunLight(eachVal);
                sunlight->setClickable(false);//设置为不可点击
                gamescene->addItem(sunlight);
                sunlight->setPos(start);
                QTimer::singleShot(1000+ var * var*10,sunlight,[=](){
                    sunlight->beCollected();
                    shop->increaseSunlight(eachVal);//shop逻辑
                });//1s后被全部统一收集
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
