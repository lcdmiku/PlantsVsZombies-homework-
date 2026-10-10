#include "snowpea.h"

SnowPea::SnowPea(QString objPath,int power)
    :Bullet(objPath,power)
{

}
void SnowPea::DealZombie(Zombie *zombie){
    Bullet::DealZombie(zombie);
    zombie->setSpeed(0.25,1500);
}
