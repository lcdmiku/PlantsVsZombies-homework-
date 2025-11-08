#include "snowpea.h"

SnowPea::SnowPea(QString objPath,int power,QTimer *moveTimer)
    :Bullet(objPath,power,moveTimer)
{

}
void SnowPea::DealZombie(Zombie *zombie){
    Bullet::DealZombie(zombie);
    zombie->setSpeed(0.25,1500);
}
