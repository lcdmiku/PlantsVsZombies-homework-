#ifndef SNOWPEA_H
#define SNOWPEA_H

#include "bullet.h"

class SnowPea : public Bullet
{
    Q_OBJECT
public:
    explicit SnowPea(QString objPath,int power);

protected:
    void DealZombie(Zombie *zombie)override;
};

#endif // SNOWPEA_H
