#include "dominator.h"
#include"gamescene.h"
#include"animate.h"
#include"sunlight.h"
#include"shop.h"
#include<QDrag>
#include<QMimeData>
#include<QMap>
#include<QTimer>


Dominator::Dominator():MyObject(nullptr,QString(":/res/GameRes/images/scarysan_small.png"),Type::Dominator),
    speed(100),speedRate(1.0),coordinate(),eventInit(false),
    sunlightTax(0), eyeProtectionOn(false), eyeProtectionLayer(nullptr),
    attachment(new QGraphicsPixmapItem(this))
{
    dialog = new DialogBox(this);//dialg 随dominator添加到场景中
    dialog->hide();
    connect(dialog,&DialogBox::branchTriggered,this,&Dominator::branchTriggered);//给外界留接口
}
//事件初始化,需要外界调用
void Dominator::initEvent(){
    if(!eventInit)
    {
        waveEvent();
        btnEvent();
        eventInit = true;
    }
}
//用于创建不同关卡的dominator行为逻辑,只在初始化时使用
//响应波次到达
void Dominator::waveEvent(){
    //征收阳光税,因为偷感十足，所以没有动画

    //一段对白的开始
    connect(this,&Dominator::waveStart,this,[=](int currwave){
        stealSunlight(sunlightTax);
        stopRandomWalk();
        Animate(this).duration(AnimationType::Move,1000)
            .move(QPointF(coordinate.getX(4),coordinate.getY(2)),false);
        QString info = "第" + QString::number(currwave) +"波僵尸即将到来";
        if(currwave > 0 && currwave <= 6)setDialog(info,{"知道了"},{-1});

        switch (currwave) {
        case 0:{
            //1
            QTimer::singleShot(1000,this,[=](){
                setDialog("你好啊杂鱼～");
                //2
                QTimer::singleShot(1500,this,[=](){
                    setDialog("现在外面的臭僵尸这么多，快要威胁到本小姐的房子了");
                    //3
                    QTimer::singleShot(2000,this,[=](){
                        setDialog("本小姐需要一位保镖来保护本小姐，你看上去有胳膊有腿的，本小姐觉得很合适");
                        //4
                        QTimer::singleShot(3000,this,[=](){
                            setDialog("如果本小姐没猜错，你应该很乐意吧～");
                            //5
                            QTimer::singleShot(2000,this,[=](){
                                QVector<QString> btnStrs;
                                QVector<int> btnIds;
                                btnStrs.push_back("豌豆射手");
                                btnStrs.push_back("土豆地雷");
                                btnStrs.push_back("樱桃炸弹");
                                btnStrs.push_back("坚果");
                                btnStrs.push_back("寒冰射手");
                                btnIds.push_back(0);
                                btnIds.push_back(1);
                                btnIds.push_back(2);
                                btnIds.push_back(3);
                                btnIds.push_back(4);
                                setDialog("本小姐先来测试一下你的能力。你从这些植物里面献祭一个，选择不同的植物会有不同的效果，还会影响到最终的考核成绩。一定要谨慎选择哦~"
                                          ,btnStrs,btnIds);
                            });
                        });
                    });

                });
            });
            break;
        }
        case 1:{
            //1
                QTimer::singleShot(15 * 1000,this,[=](){
                    QVector<QString> btnStrs;
                    QVector<int> btnIds;
                    btnStrs.push_back("借300阳光~");
                    btnStrs.push_back("我自己能行！");
                    btnIds.push_back(10);
                    btnIds.push_back(11);
                    stopRandomWalk();
                    setDialog("你带的阳光也太少了，本小姐可以大发慈悲借你一点",btnStrs,btnIds);
                });
            break;
        }
        case 2:{
            //1
            QTimer::singleShot(2 * 1000,this,[=](){
                setDialog("等会会出现带铁桶的僵尸，你应该能应付吧～");
                //2
                QTimer::singleShot(2000,this,[=](){
                    hideDialog();
                });
            });

            QTimer::singleShot(20 * 1000,this,[=](){
                QVector<QString> btnStrs;
                QVector<int> btnIds;
                btnStrs.push_back("要，帮我种几个植物");
                btnStrs.push_back("要，帮我移除几个僵尸");
                btnStrs.push_back("不需要");
                btnIds.push_back(20);
                btnIds.push_back(21);
                btnIds.push_back(22);
                setDialog("你的植物好像没什么干劲呀～需不需要本小姐的帮助？",btnStrs,btnIds);
            });
            break;
        }
        case 3:{
            //1
            QTimer::singleShot(2 * 1000,this,[=](){
                setDialog("有些抱着铁门的僵尸要来了～他们的铁门可以挡住很多豌豆子弹");
                //2
                QTimer::singleShot(1000,this,[=](){
                    hideDialog();
                });
            });

            QTimer::singleShot(30 * 1000,this,[=](){
                QVector<QString> btnStrs;
                QVector<int> btnIds;
                btnStrs.push_back("要");
                btnStrs.push_back("不需要");
                btnIds.push_back(30);
                btnIds.push_back(31);
                setDialog("本小姐有一个好主意～想不想看看本小姐的主动防御机制？",btnStrs,btnIds);
            });
            break;
        }
        case 4:{
            //1
            QTimer::singleShot(2 * 1000,this,[=](){
                setDialog("橄榄球僵尸跑得飞快，可不容易防御～");
                //2
                QTimer::singleShot(1000,this,[=](){
                    hideDialog();
                });
            });

            QTimer::singleShot(20 * 1000,this,[=](){
                QVector<QString> btnStrs;
                QVector<int> btnIds;
                setDialog("温馨提示，长时间游戏伤眼睛，本小姐来帮你开一下护眼模式。",btnStrs,btnIds);
                QTimer::singleShot(1000,this,[=](){
                    hideDialog();
                    ProtectEyes();
                });
            });
            break;
        }
        case 5:{
            //1
            QTimer::singleShot(2 * 1000,this,[=](){
                setDialog("准备好迎接更加猛烈的攻势了吗~");
                //2
                QTimer::singleShot(1000,this,[=](){
                    hideDialog();
                });
            });

            QTimer::singleShot(20 * 1000,this,[=](){
                QVector<QString> btnStrs;
                QVector<int> btnIds;
                btnStrs.push_back("给植物加点攻击");
                btnStrs.push_back("给植物加点攻速");
                btnIds.push_back(50);
                btnIds.push_back(51);
                setDialog("本小姐给你最后一次加强植物的机会",btnStrs,btnIds);
            });
            break;
        }
            //boss
        case 6:{
            QTimer::singleShot(1500,this,[=](){
                stopRandomWalk();
                sacrifyImpactBoss(sacrifiedPlant);
            });
            //1
            QTimer::singleShot(2 * 1000,this,[=](){
                setDialog("不好，是那个大块头！本小姐要躲远一点，这个家伙太丑了。");
                //2
                QTimer::singleShot(3*1000,this,[=](){
                setDialog("本小姐会在房子里看着你的～。");
                    //3
                    QTimer::singleShot(1500,this,[=](){
                        hideDialog();
                    Animate(this).duration(AnimationType::Move,1 * 1000)
                            .move(QPointF(coordinate.getX(-1)-100,coordinate.getY(2)),false);//1s逃离现场
                    });
                });
            });
            break;
        }
        default:
            break;
        }
    });
}
//响应按键event,第n波第k个事件编码 10*n + k,一般k<10
void Dominator::btnEvent(){
    GameScene* gamescene = getGameScene();
    if(!gamescene){
        qDebug()<<"dominator can't get the gamescene!error!";
        return;
    }
    connect(dialog,&DialogBox::branchTriggered,this,[=](int id){
        switch (id) {
        case -1:case -2:case -3:{
            stopRandomWalk();
            hideDialog();
            break;
        }
        case 0:case 1:case 2:case 3:case 4:
        {
            stopRandomWalk();
            PlantType sacrifice = getSacrificedFromBtnId(id);
            sacrifiedPlant=sacrifice;
            if(sacrifice != PlantType::None){
                sacrifyPlant(sacrifice);
                setDialog(sacrifiedPlantName);
                QTimer::singleShot(2000,this,[=](){
                    resetAttachment();
                    hideDialog();
                    randomWalk();
                });//1s后隐藏对话框
            }
            break;
        }
            //1
        case 10:{
            giveSunlight(10,30);
            //加税
            sunlightTax = 200;
            hideDialog();
            randomWalk();
            break;
        }
        case 11:{
            setDialog("那本小姐看看你到底有什么本事嚯嚯嚯嚯~");
            QTimer::singleShot(1500,this,[=](){
                hideDialog();
                randomWalk();
            });//1s后隐藏对话框
            break;
        }
            //2
        case 20:{
            hideDialog();
            attachment->setPixmap(QPixmap(":/res/GameRes/images/mahosteel(1).png"));
            //获取有植物的土地
            QList<PlantArea*> areas = gamescene->filterGameScene<PlantArea>([](PlantArea* area){
                return area->getIsPlantable();
            });
            if(!areas.empty()){
                for(int i=0;i<3;i++){
                    int gen = QRandomGenerator::global()->bounded(0,areas.size());
                    PlantArea *area = areas[gen];
                    PlantType planttype = getRandomPlantType();
                    int r = area->r();
                    int c = area->c();
                    QTimer::singleShot(i * (1000+100) + 100,this,[=](){
                        Animate(this).duration(AnimationType::Move,1000)
                            .move(QPointF(coordinate.getX(c),coordinate.getY(r)),false)
                            .finish(AnimationType::Move,[=](){
                            plant(planttype,r,c);
                        });
                    });
                }
            }
            QTimer::singleShot(3 * 1100 + 1000,this,[=](){
                setDialog("还不快感谢本小姐~");
                QTimer::singleShot(2000,this,[=](){
                    resetAttachment();
                    hideDialog();
                    randomWalk();
                });
            });
            break;
        }
        case 21:{
            hideDialog();
            QList<QPointF> poses;
            for (int i = 0; i < 2; ++i) {
                int r = QRandomGenerator::global()->bounded(0,5);
                int c = QRandomGenerator::global()->bounded(4,9);
                qreal y = coordinate.getY(r);
                qreal x = coordinate.getX(c);
                // ZombieGenerate(ZombieType::NormalZombie,r,x);
                poses.push_back(QPointF(x,y));
            }
            // wipeZombie(poses);

            // //如果不成功，不断抽取
            //获取有植物的土地
            QList<PlantArea*> areas = gamescene->filterGameScene<PlantArea>([](PlantArea* area){
                return !area->checkEmpty();
            });
            if(areas.empty() == false){//如果有种植植物
                int gen = QRandomGenerator::global()->bounded(0,areas.size());
                PlantArea *area = areas[gen];
                int r = area->r();
                int c = area->c();
                setAttachment(":/res/GameRes/images/Shovel.png");
                Animate(this).duration(AnimationType::Move,1000)
                    .move(QPointF(coordinate.getX(c),coordinate.getY(r)),false)
                    .finish(AnimationType::Move,[=](){
                    area->removePlant();
                    setDialog("本小姐真是大好人~");
                    resetAttachment();
                    ZombieGenerate(poses);
                    QTimer::singleShot(poses.size() * 1100 + 1100,this,[=](){
                        setDialog("我给你请了几位僵尸朋友~");
                        QTimer::singleShot(2000,this,[=](){
                            hideDialog();
                            randomWalk();
                        });
                    });

                });

            }
            else{
                ZombieGenerate(poses);
                QTimer::singleShot(poses.size() * 1100 + 1100,this,[=](){
                    setDialog("我给你请了几位僵尸朋友~");
                    QTimer::singleShot(2000,this,[=](){
                        hideDialog();
                        randomWalk();
                    });
                });
                setDialog("孤家寡人~连个植物都不愿意呆在这晦气的地方~");
            }
            break;
        }
        case 22:{
            hideDialog();
            randomWalk();
            break;
        }
            //3
        case 30:{
            //放开所有小推车
            setDialog("上吧，小推车们！怎么样？是不是很壮观～");
            QTimer::singleShot(2 * 1000,this,[=](){
                hideDialog();
                randomWalk();
            });
            if(gamescene){
                QMap<int,Mower*> mowers = gamescene->getMowers();
                for(int i=0;i<5;i++){
                    if(mowers.contains(i)){
                        releaseMower(i);
                    }
                }
            }
            break;
        }
        case 31:{
            //放开2个小推车
            setDialog("你这么厉害，本小姐要给你上点强度，不然没有好戏看会很无聊的～");
            QTimer::singleShot(2 * 1000,this,[=](){
                hideDialog();
                randomWalk();
            });
            if(gamescene){
                QMap<int,Mower*> mowers = gamescene->getMowers();
                QList<int> exist;//得到现存小推车
                for(int i=0;i<5;i++){
                    if(mowers.contains(i)){
                        exist.push_back(i);
                    }
                }
                for(int i=0;i<2;i++){
                    if(exist.size() <=0 )break;
                    int index =  QRandomGenerator::global()->bounded(0,exist.size());
                    releaseMower(exist[index]);
                }
            }

            break;
        }
            //4
        case 40:{
            //TODO
            break;
        }
            //5
        case 50:{
            //加子弹威力
            if(gamescene){
                for(int r=0;r<5;r++){
                    for(int c=0;c<9;c++){
                        PlantArea* area = gamescene->getPlantArea(r,c);
                        if(area)
                        {
                            Plant* plant = area->getPlant();
                            if(plant){
                                plant->setBulletPowerRate(2.0);
                            }
                        }

                    }
                }
            }
            hideDialog();
            randomWalk();
            break;
        }
        case 51:{
            //加子弹速度
            if(gamescene){
                for(int r=0;r<5;r++){
                    for(int c=0;c<9;c++){
                        PlantArea* area = gamescene->getPlantArea(r,c);
                        if(area)
                        {
                            Plant* plant = area->getPlant();
                            if(plant){
                                plant->setBulletSpeedRate(2.0);
                            }
                        }

                    }
                }
            }
            hideDialog();
            randomWalk();
            break;
        }
        default:
            break;
        }
    });
}
//辅助函数
PlantType Dominator::getSacrificedFromBtnId(int btnId){
    //TODO
    //你可以在这里实现boss数值逻辑
    switch (btnId) {
    case 0:
    {
        sacrifiedPlantName = QString("往日种种.....豌豆射手和你也真是一对苦命鸳鸯啊o(TヘTo)");
        setAttachment(":/res/GameRes/images/kmyy(1).png",QPointF(-100,-30));
        return PlantType::PEASHOOTER;
    }
    case 1:
    {
        sacrifiedPlantName = QString("土豆地雷的爱虽然廉价，但这已经是他的全部了");
        return PlantType::POTATOMINE;
    }

    case 2:
    {
        sacrifiedPlantName = QString("樱桃炸弹说到时候会到你家找你商量");
        return PlantType::CHEERYBOMB;
    }
    case 3:
    {
        sacrifiedPlantName = QString("愿意为你扛下所有，也要选择抛弃吗");
        return PlantType::WALLNUT;
    }
    case 4:
    {
        sacrifiedPlantName = QString("寒冰射手好像变得更冷了.....");
        return PlantType::SNOWPEASHOOTER;
    }
    default:
    {
        qDebug()<<"get no plant";
        return PlantType::None;}
    }
}
//随机植物种类
PlantType Dominator::getRandomPlantType(){
    QList<PlantType> list = {
        PlantType::PEASHOOTER,
        PlantType::SUNLIGHT,
        PlantType::WALLNUT,
        PlantType::CHEERYBOMB,
        PlantType::POTATOMINE,
        PlantType::SNOWPEASHOOTER,
        PlantType::ScaredyShroom,
        // PlantType::None//None可以用来计数
    };
    int gen = QRandomGenerator::global()->bounded(0,list.size());
    return list[gen];
}
//随机游走
void Dominator::randomWalk(){

    int row = QRandomGenerator::global()->bounded(1,5);//
    int col = QRandomGenerator::global()->bounded(0,7);
    int x = coordinate.getX(col);
    int y = coordinate.getY(row);
    qDebug()<<"x:"<<x<<"y:"<<y;
    Animate(this).speed(AnimationType::Move,speed * speedRate).move(QPointF(x,y),false)
        .finish(AnimationType::Move,[=](){
        qDebug()<<"random again";
        QTimer::singleShot(1000,this,[=]{
            randomWalk();
        });
    });
}
//停止随机游走
void Dominator::stopRandomWalk(){
    Animate(this).stop(AnimationType::Move);
}
//显示要说的话
void Dominator::setDialog(QString info,const QVector<QString>& btnStrs,const QVector<int>& btnIds){
    dialog->setDialog(info,btnStrs,btnIds);
    if (!dialog->isVisible()) {
        dialog->show();
    }
}
//隐藏对话框
void Dominator::hideDialog(){
    if (dialog->isVisible()) {
        dialog->setDialog("");//将其置空
        dialog->hide();
    }
}
//僵尸生成
void Dominator::ZombieGenerate(ZombieType zombieType,int row,int x){
    GameScene* gamescene = getGameScene();
    if(gamescene){

        gamescene->ZombieGenerate(zombieType,row,x);

    }
}
void Dominator::ZombieGenerate(QList<QPointF> poses){
    for(int i=0;i<poses.size();i++){
        attachment->setPixmap(QPixmap(":/res/GameRes/images/mahosteel(1).png"));
        QTimer::singleShot(i  * (1000 + 100) + 100,this,[=](){//没0.5秒收割一个，每次间隔0.1s
            Animate(this).duration(AnimationType::Move,1 * 1000)//0.5s 内移除
                .shape(AnimationType::Move,QEasingCurve::InOutCubic).move(poses[i],false)
                .finish(AnimationType::Move,[=](){
                    qDebug()<<"本小姐在这里";
                    // int pw = attachment->pixmap().rect().width();
                    // int ph = attachment->pixmap().rect().height();
                    // attachment->pixmap().scaled(2 * pw,2 * ph);
                    // QTimer::singleShot(500,this,[=](){
                    //     attachment->pixmap().scaled(pw,ph);
                    // });
                    ZombieGenerate(ZombieType::NormalZombie,coordinate.getRow(poses[i].y()),poses[i].x());
                });
        });
    }
    //结束后取消魔法棒
    QTimer::singleShot(poses.size() * 1100 + 1000,this,[=](){
        attachment->setPixmap(QPixmap());
    });
}
//移除僵尸
void Dominator::wipeZombie(QPointF pos,int num){
    //运动到指定位置后，删除该位置的僵尸
    GameScene* gamescene = getGameScene();
    if(gamescene){
        Animate(this).duration(AnimationType::Move,0.5 * 1000)//0.5s 内移除
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
//多位置移除僵尸
void Dominator::wipeZombie(QList<QPointF> poses){
    GameScene* gamescene = getGameScene();
    if(gamescene){
        for(int i=0;i<poses.size();i++){
            QTimer::singleShot(i * 0.5 * 1000 + 100,this,[=](){//没0.5秒收割一个，每次间隔0.1s
            Animate(this).duration(AnimationType::Move,0.5 * 1000)//0.5s 内移除
            .shape(AnimationType::Move,QEasingCurve::InOutCubic).move(poses[i],false)
                    .finish(AnimationType::Move,[=](){
                    QList<QGraphicsItem*> collisions = collidingItems();
                    foreach (QGraphicsItem* item, collisions) {
                        MyObject *zombie = dynamic_cast<Zombie*>(item);
                        if(zombie && !zombie->IsDead()){
                            gamescene->removeItem(zombie);
                            delete zombie;
                            break;
                        }
                    }
                });
            });
        }
    }
}
//种植植物
bool Dominator::plant(enum PlantType plantype,int r,int c){
    GameScene* gamescene = getGameScene();
    if(gamescene){
        PlantArea* area = gamescene->getPlantArea(r,c);
        if(area){
            return area->plant(plantype);
        }
    }
    return false;
}
//铲除植物
bool Dominator::uproot(int r,int c){
    GameScene* gamescene = getGameScene();
    if(gamescene){
        PlantArea* area = gamescene->getPlantArea(r,c);
        if(area){
            return area->removePlant();
        }
    }
    return false;
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
//小推车
void Dominator::releaseMower(int r){
    GameScene* gamescene = getGameScene();
    if(gamescene){
        gamescene->releaseMower(r);
    }
}
void Dominator::addMower(int r){
    GameScene* gamescene = getGameScene();
    if(gamescene){
        gamescene->mowerGenerate(r);
    }
}
//
//改变（r,c)位置的植物攻速
void Dominator::setPlantSpeedRate(qreal rate,int r,int c){
    GameScene* gamescene = getGameScene();
    if(gamescene){
        PlantArea* area = gamescene->getPlantArea(r,c);
        if(area){
            Plant* plant = area->getPlant();
            if(plant){
                plant->setBulletSpeedRate(rate);
            }
        }
    }
}
//改变（r,c)位置的植物威力
void Dominator::setPlantPowerRate(qreal rate,int r,int c){
    GameScene* gamescene = getGameScene();
    if(gamescene){
        PlantArea* area = gamescene->getPlantArea(r,c);
        if(area){
            Plant* plant = area->getPlant();
            if(plant){
                plant->setBulletPowerRate(rate);
            }
        }
    }
}
//使某个地方无法种植,path 是无法种植地方覆盖物的图片路径
void Dominator::setPlantabel(bool plantable,int r,int c,QString path){
    GameScene* gamescene = getGameScene();
    if(gamescene){
        PlantArea* area = gamescene->getPlantArea(r,c);
        if(area){
            area->setPlantable(plantable,path);
        }
    }
}

void Dominator::ProtectEyes() {
    if (eyeProtectionOn) return;
    eyeProtectionOn = true;

    GameScene* scene = getGameScene();
    if (!scene) return;

    // 1. 添加淡黄色薄膜
    if (!eyeProtectionLayer) {
        // 覆盖整个场景
        eyeProtectionLayer = new QGraphicsRectItem(scene->sceneRect());
        // 淡黄色 (R=255, G=255, B=200), 透明度 50 (0-255)
        eyeProtectionLayer->setBrush(QBrush(QColor(255, 128, 0, 50)));
        eyeProtectionLayer->setPen(Qt::NoPen);
        // 设置较高的 ZValue 以覆盖大部分游戏物体
        // 僵尸 ZValue 是 row (0-4). 植物也是.
        eyeProtectionLayer->setZValue(100); 
        scene->addItem(eyeProtectionLayer);
    } else {
        eyeProtectionLayer->setVisible(true);
    }

    // 2. 现有僵尸减速
    QList<Zombie*> zombies = scene->getZombies();
    for (Zombie* zombie : zombies) {
        if (zombie) {
            zombie->setSlow(true);
        }
    }

    // 3. 8秒后自动关闭
    QTimer::singleShot(8000, this, [=](){
        eyeProtectionOn = false;
        if(eyeProtectionLayer) eyeProtectionLayer->setVisible(false);
        
        GameScene* currentScene = getGameScene();
        if(currentScene){
             QList<Zombie*> currentZombies = currentScene->getZombies();
             for (Zombie* zombie : currentZombies) {
                if (zombie) {
                    zombie->setSlow(false);
                }
            }
        }
    });
}
//献祭植物,返回值代表是否献祭成功
bool Dominator::sacrifyPlant(PlantType planttype){
    GameScene* gamescene = getGameScene();

    if(gamescene){
        Shop* shop = gamescene->getShop();
        if(shop){
            QVector<Card*> cards = shop->getCards();
            for(auto card : cards){
                if(card->getPlantType() == planttype){
                    card->setUnselectable();
                    return true;
                }
            }
        }
    }
    return false;
}

void Dominator::setTemporalPix(QString pixPath,QPointF){

}

void Dominator::sacrifyImpactBoss(PlantType planttype){
    GameScene* gamescene = getGameScene();

    if(gamescene){
        switch (planttype) {
        case PlantType::PEASHOOTER:
            gamescene->levelData->setZomboniInterval(16000);
            setDialog("献祭豌豆射手 --> Boss能力-40%");
            QTimer::singleShot(3000,this,[=](){
                hideDialog();
                randomWalk();
            });
            break;
        case PlantType::POTATOMINE:
            gamescene->levelData->setZomboniHPRate(0.8);
            setDialog("献祭土豆地雷 --> Boss血量-20%");
            QTimer::singleShot(3000,this,[=](){
                hideDialog();
                randomWalk();
            });
            break;
        case PlantType::SNOWPEASHOOTER:
            gamescene->levelData->setZomboniSpeedRate(0.6);
            setDialog("献祭寒冰射手 --> Boss移速-40%");
            QTimer::singleShot(3000,this,[=](){
                hideDialog();
                randomWalk();
            });
            break;
        case PlantType::WALLNUT:
            gamescene->levelData->setZomboniHPRate(0.7);
            setDialog("献祭坚果墙 --> Boss血量-30%");
            QTimer::singleShot(3000,this,[=](){
                hideDialog();
                randomWalk();
            });
            break;
        case PlantType::CHEERYBOMB:
            gamescene->levelData->setZomboniInterval(15000);
            setDialog("献祭樱桃炸弹 --> Boss能力-25%");
            QTimer::singleShot(3000,this,[=](){
                hideDialog();
                randomWalk();
            });
            break;
        default:
            break;
        }
    }

}
