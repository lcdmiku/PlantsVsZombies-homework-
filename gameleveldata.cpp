#include "gameLevelData.h"
#include "gameScene.h"
#include <QTimer>


GameLevelData::GameLevelData():
    sunProb(20),
    backgroundImage(":/res/GameRes/images/Background.jpg"),
    backgroundMusic("qrc:/res/GameRes/audio/UraniwaNi.mp3"),//必须是url
    mowerRow({ 1, 1, 1, 1, 1, 1 }),
    hasShovel(true),
    maxSelectedCards(8),
    waveNum(5),
    waveDuration({120,120,120,120,120})
{}

void  GameLevelData::loadData(GameScene *gameScene)
{

}

void GameLevelData::startGame(GameScene *gameScene)
{

}



void GameLevelData::endGame(GameScene *gameScene)
{

}


GameLevelData_1::GameLevelData_1()
{
    eName = "1";
    cName = QString("Level 1-1");
    pName = { "Peashooter", "SnowPea", "SunFlower","WallNut","CherryBomb" ,"PotatoMine"};
    zombieData = { { ZombieType::NormalZombie,  {5,10,5,5,10} }, { ZombieType::FootballZombie, {5,10,5,5,10} },
                  { ZombieType::ScreenZombie, {5,10,5,5,10} },
                  { ZombieType::ConeZombie,  {5,10,5,5,10} }, { ZombieType::BucketZombie,  {5,10,5,5,10} } };

    largeWaveFlag = { 2,5 };

}

GameLevelData *GameLevelDataFactory(const QString &eName)
{
    if (eName == "1")
        return new GameLevelData_1();
    return nullptr;
}
