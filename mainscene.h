#ifndef MAINSCENE_H
#define MAINSCENE_H
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QMainWindow>
#include <QRandomGenerator>
#include "shop.h"
#include"sunlight.h"
#include"plantarea.h"
#include"nomalzombie.h"
#include"screenzombie.h"
#include"conezombie.h"
#include"footballzombie.h"
#include"bucketzombie.h"
#include"mower.h"
#include"selectplant.h"
#include"startwidget.h"
#include"settingsmenu.h"
#include"shovel.h"
#include<QPainter>
#include<QPixmap>
#include<QSettings>
#include<QStackedWidget>
#include<QPushButton>
#include<QHBoxLayout>
#include<QLabel>
#include<QGraphicsProxyWidget>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QSoundEffect>
#include <QAudioOutput>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainScene;
}
QT_END_NAMESPACE

class MainScene : public QMainWindow
{
    Q_OBJECT

public:
    MainScene(QWidget *parent = nullptr);
    Shop *getShop(){return shop;}
    ~MainScene();

signals:
    void GamePause();
    void GameContinue();
    void GameOver();

private:
    Ui::MainScene *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    Shop *shop;
    SelectPlant *selectPlant;
    QSettings *settings;
    QMediaPlayer *bgMus;
    QAudioOutput *audioOutput;
    Shovel * shovel;
    SettingsMenu *settingsMenu;
    //QTimer
    QTimer *moveTimer;
    QTimer *timer;
    //栈UI
    QStackedWidget *stackedWidget;
    StartWidget *startWidget;

    void PlantAreaGenerate();
    void ZombieGenerate();
    void cardAvailable();
    void GameStart();
    void GamePre();


};


#endif // MAINSCENE_H
