#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <QObject>
#include<QMap>
#include<QHash>
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QSoundEffect>
#include<QMutex>
class GameScene;
class SoundManager
{

    const int MAXCACHE = 10;
    GameScene* gamescene;
    QMap<QString,QList<QMediaPlayer*>> players;
    QMap<QString,QList<QAudioOutput*>> audios;
    QHash<QString,QList<QSoundEffect*>> soundEffects;
    //xian c suo
    QMutex cacheMutex;

public:
    //播放短时音效
    void playSoundEffect(const QString& soundPath);
    SoundManager(GameScene* gamescene);
    ~SoundManager();
};

#endif // SOUNDMANAGER_H
