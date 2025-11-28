#include "soundmanager.h"
#include"gamescene.h"
#include<QThread>
#include<QFile>

SoundManager::SoundManager(GameScene* gamescene):gamescene(gamescene),
    players(),audios(),soundEffects(),usingSounds()
{
    qputenv("QT_MEDIA_DEBUG", "0");
    qputenv("QT_SOUND_EFFECT_DEBUG", "0");
    initSound("qrc:/res/GameRes/audio/lawnmower.wav");
    initSound("qrc:/res/GameRes/audio/plant1.wav");
    initSound("qrc:/res/GameRes/audio/points.wav");
    initSound("qrc:/res/GameRes/audio/winmusic.mp3");

}

void SoundManager::playSoundEffect(const QString& soundPath){
    mediaSoundPlay(soundPath);
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

void SoundManager::initSound(const QString& soundPath){
    if(!soundEffects.contains(soundPath)){
        QList<QSoundEffect*> cacheList;
        for(int i=0;i<MAXCACHE;i++){
            QSoundEffect* sound = new QSoundEffect();
            sound->setLoopCount(1);
            sound->setVolume(0.5);
            cacheList.push_back(sound);
            // 关键：把setSource放到主线程事件队列，分散加载压力（不瞬时阻塞）
            QMetaObject::invokeMethod(sound, [=]() {
                sound->setSource(QUrl(soundPath)); // 路径规范化
            }, Qt::QueuedConnection);
        }
        soundEffects[soundPath] = cacheList;
        usingSounds[soundPath] = 0; // 初始化计数
    }
}

void SoundManager::cacheSoundPlay(const QString& soundPath){
    if(!gamescene){
        qDebug()<<"SoundManager:gamescene is nullptr";
        return;
    }
    if (soundPath.isEmpty()) {
        qWarning() << "SoundManager: invalid sound path:" << soundPath;
        return;
    }

    QMutexLocker locker(&cacheMutex);
    if(!soundEffects.contains(soundPath)){
        QList<QSoundEffect*> cacheList;
        for(int i=0;i<MAXCACHE;i++){
            QSoundEffect* sound = new QSoundEffect();
            sound->setLoopCount(1);
            sound->setVolume(0.5);
            cacheList.push_back(sound);
            QMetaObject::invokeMethod(sound, [=]() {
                sound->setSource(QUrl(soundPath));
            }, Qt::QueuedConnection);
        }
        soundEffects[soundPath] = cacheList;
        usingSounds[soundPath] = 0;
    }
    QList<QSoundEffect*> Cache = soundEffects[soundPath];
    int currentIdx = usingSounds[soundPath] % MAXCACHE;
    QSoundEffect* sound = Cache[currentIdx];
    usingSounds[soundPath] = (usingSounds[soundPath] + 1) % MAXCACHE;
    locker.unlock();
    auto safePlay = [=]() {
        if (!sound) return;
        if (sound->status() == QSoundEffect::Ready) {
            sound->play();
        }
        /*else if (sound->status() == QSoundEffect::Loading) {
            static QMetaObject::Connection conn;
            QObject::disconnect(conn);
            conn = QObject::connect(sound, &QSoundEffect::statusChanged, [=]() {
                if (sound->status() == QSoundEffect::Ready) {
                    sound->play();
                    QObject::disconnect(conn);
                }
            });
        }*/

    };
    if(QThread::currentThread() != this->thread()){
        QMetaObject::invokeMethod(sound, safePlay, Qt::QueuedConnection);
    } else {
        safePlay();
    }
}
// QSoundEffect* sound = new QSoundEffect(gamescene);
// soundEffects[soundPath].push_back(sound);
// sound->setLoopCount(1);
// sound->setVolume(0.5);
// sound->setSource(QUrl(soundPath));
// //不够就加量
// Cache.push_back(sound);
// sound->play();


void SoundManager::disposalPlay(const QString& soundPath){

    if(usingSounds.contains(soundPath) && usingSounds[soundPath]>MAXCACHE){
        return;
    }
    usingSounds[soundPath]++;//防止加载过多
    QSoundEffect *soundEffect = new QSoundEffect();
    // 设置声音源文件的路径
    soundEffect->setSource(QUrl(soundPath));
    // 音频循环的次数
    soundEffect->setLoopCount(1);
    // 音量
    soundEffect->setVolume(1);
    soundEffect->play();

    // 连接信号，当播放完毕时，自动销毁对象。
    QSoundEffect::connect(soundEffect, &QSoundEffect::playingChanged, [=] () {
        if (!soundEffect->isPlaying()) {
            usingSounds[soundPath]--;
            soundEffect->deleteLater();
        }
    });
}

void SoundManager::mediaSoundPlay(const QString& soundPath){
    QMediaPlayer* player = new QMediaPlayer(gamescene);
    QAudioOutput* audioOutput = new QAudioOutput(gamescene);
    players[soundPath].push_back(player);
    audios[soundPath].push_back(audioOutput);
    //
    audioOutput->setVolume(0.5);
    player->setAudioOutput(audioOutput);
    player->setSource(soundPath);
    player->play();

    QMediaPlayer::connect(player,&QMediaPlayer::playbackStateChanged,player,[=](){
        if(player->playbackState() == QMediaPlayer::StoppedState){
            player->deleteLater();
            audioOutput->deleteLater();
        }
    });
}

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
