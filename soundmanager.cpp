#include "soundmanager.h"
#include"gamescene.h"
SoundManager::SoundManager(GameScene* gamescene):gamescene(gamescene),
    players(),audios(),soundEffects()
{}

void SoundManager::playSoundEffect(const QString& soundPath){
    //使用soundEffect
    if(!gamescene){
        qDebug()<<"SoundManager:gamescene is nullptr";
        return;
    }
    if(!soundEffects.contains(soundPath)){
        for(int i=0;i<MAXCACHE;i++){
            QSoundEffect* sound = new QSoundEffect(gamescene);
            soundEffects[soundPath].push_back(sound);
            sound->setLoopCount(1);
            sound->setVolume(0.5);
            sound->setSource(soundPath);
        }
    }//进行cache
    //
    QList<QSoundEffect*> Cache = soundEffects[soundPath];
    for(int i=0;i<MAXCACHE;i++){
        QSoundEffect* sound = Cache[i];
        if(sound->status() == QSoundEffect::Ready){
            sound->play();
            break;
        }
    }
}


// if(!players.contains(soundPath)){
//     for(int i=0;i<MAXCACHE;i++){
//         QMediaPlayer* player = new QMediaPlayer(gamescene);
//         QAudioOutput* audioOutput = new QAudioOutput(gamescene);
//         players[soundPath].push_back(player);
//         audios[soundPath].push_back(audioOutput);
//         //
//         audioOutput->setVolume(0.5);
//         player->setAudioOutput(audioOutput);
//         player->setSource(soundPath);
//     }
// }//进行cache
// QList<QMediaPlayer*> playerCache = players[soundPath];
// //
// for(int i=0;i<MAXCACHE;i++){
//     if(playerCache[i]->playbackState()!=QMediaPlayer::PlayingState){
//         playerCache[i]->play();
//         break;
//     }
// }
