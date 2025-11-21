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
#include"selectlevelwidget.h"
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
#include"gamescene.h"
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
    ~MainScene();

signals:
    void GamePause();
    void GameContinue();
    void GameOver();

private:
    Ui::MainScene *ui;
    //
    GameScene *scene;
    QGraphicsView *view;
   //
    SettingsMenu *settingsMenu;
    //栈UI
    QStackedWidget *stackedWidget;
    StartWidget *startWidget;
    SelectLevelWidget *selectLevelWidget;

    void GamePre();


};


#endif // MAINSCENE_H
