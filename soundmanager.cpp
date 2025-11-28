#include "soundmanager.h"
#include"gamescene.h"

SoundManager::SoundManager(GameScene* gamescene):gamescene(gamescene),
    players(),audios(),soundEffects()
{
    qputenv("QT_MEDIA_DEBUG", "0");
    qputenv("QT_SOUND_EFFECT_DEBUG", "0");
}

void SoundManager::playSoundEffect(const QString& soundPath){
    QSoundEffect *soundEffect = new QSoundEffect();
    // 设置声音源文件的路径
    soundEffect->setSource(QUrl(soundPath));
    // 音频循环的次数
    soundEffect->setLoopCount(1);
    // 音量
    soundEffect->setVolume(1);
    soundEffect->play();

    // 连接信号，当播放完毕时，自动销毁对象。
    QSoundEffect::connect(soundEffect, &QSoundEffect::playingChanged, [soundEffect] () {
        if (soundEffect->isPlaying()) {
            soundEffect->deleteLater();
        }
    });
    // QMediaPlayer* player = new QMediaPlayer(gamescene);
    // QAudioOutput* audioOutput = new QAudioOutput(gamescene);
    // players[soundPath].push_back(player);
    // audios[soundPath].push_back(audioOutput);
    // //
    // audioOutput->setVolume(0.5);
    // player->setAudioOutput(audioOutput);
    // player->setSource(soundPath);
    // player->play();

    // QMediaPlayer::connect(player,&QMediaPlayer::playbackStateChanged,player,[=](){
    //     if(player->playbackState() == QMediaPlayer::StoppedState){
    //         player->deleteLater();
    //         audioOutput->deleteLater();
    //     }
    // });
}
SoundManager::~SoundManager() {
    QMutexLocker locker(&cacheMutex);  // 加锁保证线程安全
    // 遍历所有缓存的音效列表，逐个删除
    for (auto& soundList : soundEffects) {
        qDeleteAll(soundList);  // 删除QSoundEffect对象
        soundList.clear();      // 清空列表
    }
    soundEffects.clear();       // 清空哈希表
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



// //使用soundEffect
// if(!gamescene){
//     qDebug()<<"SoundManager:gamescene is nullptr";
//     return;
// }
// QMutexLocker locker(&cacheMutex);
// if(!soundEffects.contains(soundPath)){
//     for(int i=0;i<MAXCACHE;i++){
//         QSoundEffect* sound = new QSoundEffect(gamescene);
//         soundEffects[soundPath].push_back(sound);
//         sound->setLoopCount(1);
//         sound->setVolume(0.5);
//         sound->setSource(QUrl(soundPath));
//     }
// }//进行cache
// //
// QList<QSoundEffect*> Cache = soundEffects[soundPath];
// for(int i=0;i<MAXCACHE;i++){
//     QSoundEffect* sound = Cache[i];
//     if(sound->status() == QSoundEffect::Ready){
//         sound->play();
//         return;
//     }
//     if(sound->status() == QSoundEffect::Error){
//         break;
//     }
// }
// QSoundEffect* sound = new QSoundEffect(gamescene);
// soundEffects[soundPath].push_back(sound);
// sound->setLoopCount(1);
// sound->setVolume(0.5);
// sound->setSource(QUrl(soundPath));
// //不够就加量
// Cache.push_back(sound);
// sound->play();
