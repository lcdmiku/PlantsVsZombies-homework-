#ifndef MYOBJECT_H
#define MYOBJECT_H

#include<QMovie>
#include <QGraphicsObject>
#include<QPainter>
#include <QRandomGenerator>
#include<QPropertyAnimation>
#include<QTimer>
#include<QGraphicsScene>
class GameScene;

class Animate;

enum class Type{
    PLANT,
    ZOMBIE,
    Bullet,
    Mower,
    Bg,
    Dominator,
    Other
};
enum class ZombieType{
    None,
    NormalZombie,
    ScreenZombie,
    BucketZombie,
    ConeZombie,
    FootballZombie
};

enum class DieType{
    None,
    Normal,
    CherryBOMB
};
enum class Direction{
    None,
    Up,
    Down,
    Left,
    Right
};

class MyObject : public QGraphicsObject
{
    Q_OBJECT
    QMovie *movie;
    //用来时刻进行碰撞检查



public:
    explicit MyObject(QGraphicsObject *parent,QString objPath = "",const enum Type type = Type::Other);
    // 必须实现的虚函数
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    //static int sunlightGenerate();
    enum Type getObjType(){return type;}
    bool IsDead(){return isDead;}
    ~MyObject();

protected:
    GameScene* gameScene;
    GameScene* getGameScene();//得到场景数据函数
    const enum Type type;
    const QString objPath;
    QString CurrentGif;
    bool isDead;//判断是否死亡
    //
    void ToCurrentGif();

    void CheckMeet();
    void changeGif(QString newPath);
    virtual void setCurrentGif();//设计各种情况的图片gif,或许可以利用来进行类继承
    void setGif(QString newPath);//
    QTimer *timer;
signals:
    void Meet(MyObject *Meeter);
    void movieFinished();
    void GameOver();
    void needToDel();
public slots:
    virtual void GamePause();
    virtual void GameContinue();

protected slots:
    virtual void frameChanged(int frameNumber);

};

#endif // MYOBJECT_H
