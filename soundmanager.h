#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <QObject>
#include<QMap>
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QSoundEffect>

class GameScene;
class SoundManager
{
    const int MAXCACHE = 10;
    GameScene* gamescene;
    QMap<QString,QList<QMediaPlayer*>> players;
    QMap<QString,QList<QAudioOutput*>> audios;
    QMap<QString,QList<QSoundEffect*>> soundEffects;

public:
    //播放短时音效
    void playSoundEffect(const QString& soundPath);
    SoundManager(GameScene* gamescene);
};

#endif // SOUNDMANAGER_H
