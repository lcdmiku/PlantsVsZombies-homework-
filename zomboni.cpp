#include "zomboni.h"
#include "gamescene.h"
#include <QDebug>
#include <QTimer>
#include <QRandomGenerator>

Zomboni::Zomboni()
    : Zombie(":/res/GameRes/images/Zombies/Zomboni/0.gif",
             ZombieType::Zomboni,
             "",
             29999, // HP
             29,   // Speed
             10000) // Attack Power (秒杀)
{
    // 冰车僵尸通常免疫减速（可选实现，这里先保留默认行为）
    
    // 初始化技能计时器
    skillTimer = new QTimer(this);
    connect(skillTimer, &QTimer::timeout, this, &Zomboni::summonZombies);
    skillTimer->start(summonInterval); // 使用静态变量设置的时间
}

Zomboni::~Zomboni()
{
}

void Zomboni::Action(Plant *plant)
{
    // 冰车僵尸不进行常规攻击（啃食），而是直接碾压
    if (plant) {
        crushPlant(plant);
    }
}

void Zomboni::crushPlant(Plant *plant)
{
    if (!plant) return;

    // 造成巨大伤害，直接摧毁植物
    // 注意：地刺王(Spikerock)可能会对冰车造成伤害并爆胎，这里暂不实现复杂交互
    plant->Attacted(attackPower);
    
    // 冰车继续移动，不需要 stopMoving()
}

void Zomboni::setCurrentGif()
{
    if (isDead) return;

    // 根据血量切换状态 0-5
    int index = 0;
    double rate = (double)currentHp / getMaxHp();

    if (rate > 0.83) index = 0;
    else if (rate > 0.66) index = 1;
    else if (rate > 0.50) index = 2;
    else if (rate > 0.33) index = 3;
    else index = 4;

    QString newGif = QString(":/res/GameRes/images/Zombies/Zomboni/%1.gif").arg(index);
    
    // 如果 GIF 路径改变了，则更新
    if (CurrentGif != newGif) {
        CurrentGif = newGif;
        // 调用父类或 MyObject 的 changeGif 方法来刷新动画
        // 注意：MyObject::changeGif 会重置 movie
        changeGif(CurrentGif);
    }
}

void Zomboni::dealDead(enum DieType dieType)
{
    // 停止技能计时器
    if (skillTimer->isActive()) {
        skillTimer->stop();
    }

    QString DieGif;
    if(dieType == DieType::CherryBOMB){
        DieGif = ":/res/GameRes/images/Zombies/Zomboni/BoomDie.gif";
    }
    else DieGif = ":/res/GameRes/images/Zombies/Zomboni/5.gif";
    
    changeGif(DieGif);

    // 停止移动
    stopMoving();

    // 播放完动画后删除
    // 连接 movieFinished 信号 (MyObject 会转发 QMovie::finished 信号)
    // 使用 QMetaObject::Connection 防止多次连接
    QMetaObject::Connection *conn = new QMetaObject::Connection;
    *conn = connect(this, &MyObject::movieFinished, this, [=](){
        disconnect(*conn);
        delete conn;
        deleteLater();
    });

    // 兜底策略，防止动画不触发 finished (例如循环动画或加载失败)
    QTimer::singleShot(2000, this, [=](){
        if(conn) {
            disconnect(*conn);
            delete conn;
        }
        deleteLater();
    });
}

QPainterPath Zomboni::shape() const
{
    QPainterPath path;
    // 冰车碰撞箱，比普通僵尸大
    // 假设中心为原点，向左延伸（车头），向下延伸（车身）
    // 根据图片大概估算
    path.addRect(-100, -40, 50, 80);
    return path;
}

void Zomboni::summonZombies()
{
    if (isDead) return;
    
    // 获取 GameScene 指针
    // 注意：MyObject::gameScene 是 protected 的，可以直接访问
    // 但为了安全，最好重新获取一次或确保它有效
    if (!gameScene) {
        gameScene = getGameScene();
        if (!gameScene) return;
    }

    this->stopMoving();
    // 计算当前所在的行 (近似计算)
    // y = 100 + 94 * row  =>  row = (y - 100) / 94
    int myRow = (y() - 100) / 94;
    if (myRow < 0) myRow = 0;
    if (myRow > 4) myRow = 4;

    QList<QPointF> spawnPoints;
    QList<int> spawnRows;

    // 遍历周围 5x5 范围内的 PlantArea
    // 行范围：[myRow-2, myRow+2]
    for (int r = 0; r < 5; ++r) {
        if (abs(r - myRow) > 2) continue;

        for (int c = 0; c < 9; ++c) {
            PlantArea* pa = gameScene->getPlantArea(r, c);
            if (pa) {
                // 检查水平距离
                // 假设 PlantArea 宽度约 80，5x5 意味着左右各 2 格，即距离 < 200
                if (abs(pa->x() - this->x()) < 200) {
                    spawnPoints.append(QPointF(pa->x(), pa->y()));
                    spawnRows.append(r);
                }
            }
        }
    }

    if (spawnPoints.isEmpty()) return;

    // 随机生成 10-15 个僵尸
    int count = QRandomGenerator::global()->bounded(10, 16);
    
    // 定义可生成的僵尸类型列表 (排除 None 和 Zomboni 自身)
    QList<ZombieType> validTypes = {
        ZombieType::NormalZombie,
        ZombieType::ScreenZombie,
        ZombieType::BucketZombie,
        ZombieType::ConeZombie,
        ZombieType::FootballZombie,
        ZombieType::Zomboni
    };

    for (int i = 0; i < count; ++i) {
        int idx = QRandomGenerator::global()->bounded(spawnPoints.size());
        int r = spawnRows[idx];
        qreal x = spawnPoints[idx].x();
        
        // 随机选择一种僵尸类型
        ZombieType randomType;
        
        // 检查是否生成冰车自身
        if (QRandomGenerator::global()->bounded(100) < selfSummonProbability) {
            randomType = ZombieType::Zomboni;
        } else {
            int typeIndex = QRandomGenerator::global()->bounded(validTypes.size());
            randomType = validTypes[typeIndex];
        }

        // 调用 GameScene 的 ZombieGenerate
        // 注意：ZombieGenerate(type, row, x) 会设置位置为 (x, 100 + 94*row)
        // 我们传入 PlantArea 的 x 坐标，这样僵尸就会生成在 PlantArea 所在的列
        gameScene->ZombieGenerate(randomType, r, x);
    }

    QTimer::singleShot(2000, this, [=](){
        this->continueMoving();
    });
}

// 初始化静态成员变量
int Zomboni::summonInterval = 10000;
int Zomboni::selfSummonProbability = 0.2; // 默认0%

void Zomboni::setSummonInterval(int interval)
{
    summonInterval = interval;
}

int Zomboni::getSummonInterval()
{
    return summonInterval;
}

void Zomboni::setSelfSummonProbability(int prob)
{
    if (prob < 0) prob = 0;
    if (prob > 100) prob = 100;
    selfSummonProbability = prob;
}

int Zomboni::getSelfSummonProbability()
{
    return selfSummonProbability;
}
