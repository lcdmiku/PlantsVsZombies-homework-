#ifndef MOWER_H
#define MOWER_H
#include"myobj.h"
#include"zombie.h"

class Zombie;
class Mower : public MyObject
{
    Q_OBJECT
    int power;//威力
    QTimer *moveTimer;
    int Hz;
    double speed;
    bool hasCollided;
public:
    explicit Mower();
    void advance(int phase) override;

    //放生小推车
    void releaseMower(){hasCollided = true;}
    ~Mower();


protected:

signals:


};

#endif // MOWER_H
