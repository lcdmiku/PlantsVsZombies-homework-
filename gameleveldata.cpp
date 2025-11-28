#include "gameLevelData.h"
#include "gameScene.h"
#include <QTimer>
#include"dominator.h"


GameLevelData::GameLevelData():
    sunProb(5),
    backgroundImage(":/res/GameRes/images/Background.jpg"),
    backgroundMusic("qrc:/res/GameRes/audio/UraniwaNi.mp3"),//必须是url
    mowerRow({ 1, 1, 1, 1, 1, 1 }),
    hasShovel(true),
    maxSelectedCards(8),
    waveNum(7),
    waveTotWeight(waveNum,0),

    waveDuration({10,60,60,60,60,60,60}),
    waveLimits({10,12,20,32,48,60,10}),

    map(5,QList<int>(9,0)),
    zomboniSummonInterval(10000), // 默认10秒
    zomboniSelfSummonProb(5),      // 默认0%
    zomboniHPRate(1.0), // 默认100%
    zomboniSpeedRate(1.0) // 默认100%
{

}
//在僵尸池里抽取僵尸
QList<ZombieType> GameLevelData::zombieExtract(int currWave){
    QList<ZombieType> res;//结果容器
    if(currWave >= waveTotWeight.size() || currWave >= waveLimits.size()){
        qDebug()<<"currWave >= waveTotWeight.size()";
        return res;
    }
    int totWei = waveTotWeight[currWave];
    int powerLimit = waveLimits[currWave];
    int currPower = 0;
    int Wei = 0;
    QList<int> rank(zombieData.size(),0);//rank[i]是前i+1个僵尸权重和，方便后边抽取判定

    for(int i=0;i<zombieData.size();i++){
        Wei += zombieData[i].waveWeight[currWave];
        rank[i] = Wei;
    }
    while (currPower != powerLimit) {
        int gen = QRandomGenerator::global()->bounded(0,totWei);
        for(int i=0;i<zombieData.size();i++){
            //修复权重为零时不生成僵尸逻辑
            if(rank[i] > gen && zombieData[i].waveWeight[currWave]) {
                if(zombieData[i].power + currPower <= powerLimit){
                    res.push_back(zombieData[i].eName);
                    currPower += zombieData[i].power;
                }else break;//超过powerlimit需要重新抽取
            }
        }
    }
    return res;
}
void GameLevelData::waveTotWeightInit(){
    if(waveTotWeight.size() != waveNum){
        qDebug()<<"GameLevelData::waveTotWeightInit:waveTotWeight.size() != waveNum";
        return;
    }
    for (int var = 0; var < waveNum; ++var) {
        int totWei = 0;
        for(const auto& data : zombieData){
            totWei += data.waveWeight[var];
        }
        waveTotWeight[var] = totWei;
    }
}
//处理每一波dominator的行动逻辑
void GameLevelData::dominatorAct(GameScene *gameScene){
    // if(!gameScene)return;
    // int currwave = gameScene->getCurrWave();
    // Dominator* role = gameScene->getDominator();

    // switch (currwave) {
    // case 0:{
    //     //1
    //     QTimer::singleShot(1000,role,[=](){
    //         role->setDialog("有趣的小杂鱼~偏偏喜欢在有这么多恶心虫子的草坪上睡觉~");
    //         //2
    //         QTimer::singleShot(2000,role,[=](){
    //             QVector<QString> btnStrs;
    //             QVector<int> btnIds;
    //             btnStrs.push_back("你谁啊");
    //             btnIds.push_back(0);
    //             role->setDialog("本小姐都不知道怎么把你叫醒~",btnStrs,btnIds);
    //         });
    //     });

    //     break;
    // }
    // default:
    //     break;
    // }
    //将相关逻辑放入dominator类
}

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
    pName = { "Peashooter", "SnowPea", "SunFlower","WallNut","CherryBomb" ,"PotatoMine","ScaredyShroom"};


//注意：每波必须要有强度为1的僵尸否则可能因为永远无法使总强度为limitPower而引发无限循环

    zombieData = { { ZombieType::NormalZombie,  1,{10,70,50,40,30,25,10} }, { ZombieType::FootballZombie, 4,{0,0,0,0,10,15,0} },
                  { ZombieType::ScreenZombie, 3,{0,0,0,10,15,15,0} },
                  { ZombieType::ConeZombie, 2, {0,30,30,30,25,25,0} }, { ZombieType::BucketZombie, 3, {0,0,20,20,20,20,0} } };


    largeWaveFlag = { 2,6 };

    waveTotWeightInit();//初始化每波总权重

}

GameLevelData *GameLevelDataFactory(const QString &eName)
{
    if (eName == "1")
        return new GameLevelData_1();
    return nullptr;
}

//此接口预留来设置冰车僵尸难度(时间间隔，生成自身的概率)
void GameLevelData::setZomboniInterval(int interval)
{
    zomboniSummonInterval = interval;
}

void GameLevelData::setZomboniProb(int selfProb)
{
    zomboniSelfSummonProb = selfProb;
}

void GameLevelData::setZomboniHPRate(double rate)
{
    zomboniHPRate = rate;
}

void GameLevelData::setZomboniSpeedRate(double rate)
{
    zomboniSpeedRate = rate;
}
