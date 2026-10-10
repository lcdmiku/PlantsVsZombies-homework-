#include "flagmeter.h"
#include <QDebug>

FlagMeter::FlagMeter(GameLevelData* data) : levelData(data), totalDuration(0)
{
    // 加载图片
    QPixmap barPixmap(":/res/GameRes/images/FlagMeterEmpty.png");
    QPixmap headPixmap(":/res/GameRes/images/FlagMeterParts1.png");
    QPixmap flagPixmap(":/res/GameRes/images/FlagMeterParts2.png");

    if (barPixmap.isNull()) qDebug() << "Failed to load FlagMeterEmpty.png";
    if (headPixmap.isNull()) qDebug() << "Failed to load FlagMeterParts1.png";
    if (flagPixmap.isNull()) qDebug() << "Failed to load FlagMeterParts2.png";

    // 背景图
    progressBar = new QGraphicsPixmapItem(barPixmap);
    addToGroup(progressBar);
    barWidth = barPixmap.width();

    // 绿色进度条
    progressGreenBar = new QGraphicsRectItem(0, 0, 0, barPixmap.height() * 0.6); // 高度稍微小一点，或者填满
    // 颜色：PVZ 绿色大概是 #00FF00 或者稍微深一点，这里用亮绿色
    progressGreenBar->setBrush(QBrush(QColor(0, 255, 0)));
    progressGreenBar->setPen(Qt::NoPen);
    // 初始位置在最右侧，宽度为0
    progressGreenBar->setPos(barWidth, (barPixmap.height() - progressGreenBar->rect().height()) / 2);
    addToGroup(progressGreenBar);

    qDebug() << "flagMeter已经创建了";

    // 计算总时长
    if (levelData) {
        for (int duration : levelData->waveDuration) {
            totalDuration += duration;
        }
        
        // 放棋子
        int currentDuration = 0;
        for(int i = 0; i <= levelData->waveNum; ++i) {
             //有大波的时候(levelData里标记的largeWaveFlag)
            if(levelData->largeWaveFlag.contains(i)) {
                // 根据当前时长计算位置比例
                double ratio = 0;
                if(totalDuration > 0)
                    ratio = (double)currentDuration / totalDuration;
                
                // 右边开始，左边结束
                // x = 宽度 * (1 - ratio)
                double x = barWidth * (1.0 - ratio);
                
                QGraphicsPixmapItem* flag = new QGraphicsPixmapItem(flagPixmap);
                // 稍微往上抬一点位置

                flag->setPos(x, -flagPixmap.height()/2);
                addToGroup(flag);
                flags.append(flag);
            }
             
             if(i < levelData->waveDuration.size()){
                currentDuration += levelData->waveDuration[i];
             }
        }
    }

    // 僵尸头
    zombieHead = new QGraphicsPixmapItem(headPixmap);
    // 右侧开始
    zombieHead->setPos(barWidth - headPixmap.width()/2, -headPixmap.height()/2); 
    addToGroup(zombieHead);
    
    // 最初隐藏
    setVisible(false);
    setZValue(100); // Ensure it's on top
}

void FlagMeter::updateProgress(int currentMoment) { 
    if (totalDuration <= 0) return;
    // 这个函数随着gamescene里的waveTimer一起每秒调用一次，不断更新位置。
    
    // 超过总时间，那就不动。
    if (currentMoment > totalDuration) currentMoment = totalDuration;
    
    double ratio = (double)currentMoment / totalDuration;
    // 同理计算
    double x = barWidth * (1.0 - ratio);
    
    // 更新僵尸头位置
    zombieHead->setX(x - zombieHead->pixmap().width()/2);

    // 更新绿色进度条
    // 进度条从僵尸头位置(x)延伸到最右侧(barWidth)
    // 宽度 = barWidth - x
    // 位置 = x
    // 高度保持不变
    double greenWidth = barWidth - x;
    progressGreenBar->setRect(0, 0, greenWidth, progressGreenBar->rect().height());
    progressGreenBar->setPos(x, progressGreenBar->y());
}

void FlagMeter::showMeter() {
    qDebug() << "showMeter被调用了";
    setVisible(true);
}
