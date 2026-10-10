#ifndef FLAGMETER_H
#define FLAGMETER_H

#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>
#include "gameleveldata.h"

class FlagMeter : public QGraphicsItemGroup
{
public:
    explicit FlagMeter(GameLevelData* data);
    void updateProgress(int currentMoment);
    void showMeter();

private:
    GameLevelData* levelData;
    QGraphicsPixmapItem* progressBar;
    QGraphicsPixmapItem* zombieHead;
    QGraphicsRectItem* progressGreenBar; // 绿色进度条
    QList<QGraphicsPixmapItem*> flags;
    
    int totalDuration;
    qreal barWidth;
};

#endif // FLAGMETER_H
