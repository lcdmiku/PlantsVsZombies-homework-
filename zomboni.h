#ifndef ZOMBONI_H
#define ZOMBONI_H

#include "zombie.h"

class Zomboni : public Zombie
{
    Q_OBJECT
public:
    explicit Zomboni();
    ~Zomboni();

    // 调整僵尸生成速率的接口，备用
    static void setSummonInterval(int interval);
    static int getSummonInterval();

    // 设置召唤时生成冰车自身的概率，也备用
    static void setSelfSummonProbability(int prob);
    static int getSelfSummonProbability();

protected:
    // 僵尸行为特征
    void Action(Plant *plant) override;
    void setCurrentGif() override;
    void dealDead(enum DieType dieType) override;
    
    // 冰车特有的压扁植物逻辑
    void crushPlant(Plant *plant);

    // 重写碰撞区域
    QPainterPath shape() const override;

private slots:
    void summonZombies();

private:
    QTimer *skillTimer;
    static int summonInterval;
    static int selfSummonProbability;
};

#endif // ZOMBONI_H
