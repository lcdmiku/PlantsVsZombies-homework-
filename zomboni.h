#ifndef ZOMBONI_H
#define ZOMBONI_H

#include "zombie.h"

class Zomboni : public Zombie
{
    Q_OBJECT
public:
    explicit Zomboni();
    ~Zomboni();

    // 设置当前冰车的召唤间隔，未来可以通过levelData中的接口来更新这两个数据(冰车只通过指定出现)
    void setMySummonInterval(int interval);
    // 设置当前冰车的自我召唤概率
    void setMySelfSummonProbability(int prob);

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
    int mySummonInterval; // 实例变量
    int mySelfSummonProbability; // 实例变量
};

#endif // ZOMBONI_H
