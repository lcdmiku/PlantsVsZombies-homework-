#ifndef ZOMBIE_H
#define ZOMBIE_H
#include"myobj.h"
#include"plant.h"
#include<QGraphicsColorizeEffect>
#include<QDebug>

class Zombie : public MyObject
{
    Q_OBJECT
    enum ZombieType zombieType;
    QPointF startPos;
    QPointF endPos;
    QString attackingGif;
    int hp;//hp
    int Hz;//僵尸走动帧率
public:
    explicit Zombie(QString objPath,
                    enum ZombieType zombieType,QString attackingGif,int hp,int speed,int attackpower);
    //与植物交互效果
    void beHeated(int power,enum DieType dieType);
    //将各种伤害独立成一个函数，beHeated仅提供扣血
    void getPea(int power);
    void getSnowPea(int power);

    void setSpeed(double rate,int duration);
    void changeRow(enum Direction dir);
    //
    //给外界的信息
    int getSpeed(){return speed;}
    ~Zombie();

    static int zombieNum;

signals:
    //僵尸胜利
    void zombieSuccess();
    void noZombie(QPointF pos);//当场上没有僵尸时触发

    void dead();



protected:
    int currentHp;
    bool movable;
    double speed;
    double CurrentSpeedRate;
    int attackPower;
    QTimer *zombieAttackTimer;//需要计时器控制
    QTimer *IfwinTimer;//判断是否进屋
    QPointer<Plant> attackedPlant;//使用弱指针管理被打击的植物
    QMovie *secondaryDyingMovie;
    //尝试同步计时


    // 用于显示动画的图形项
    QGraphicsPixmapItem *secondaryGifItem;
    QGraphicsColorizeEffect *hitEffect;//受伤效果
    int slowEffect;//减速效果



    //需要重写
    // QRectF boundingRect() const override;
    // 碰撞区域（仅躯干部分）
    QPainterPath shape() const override ;


    //访问成员数据
    int getMaxHp(){return hp;};
    //僵尸统一行为特征

    void stopMoving();
    void continueMoving();
    void attack(Plant *plant);
    //
    void setCurrentGif()override;
    //僵尸行为特征
    virtual void Action(Plant *plant) = 0;
    virtual void dealDead(enum DieType dieType);


private slots:


};
#endif // ZOMBIE_H
