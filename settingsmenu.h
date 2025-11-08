#ifndef SETTINGSMENU_H
#define SETTINGSMENU_H

#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QDialog>
#include <QSlider>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QDebug>


class SettingsMenu : public QDialog
{
    Q_OBJECT
    //布局
    QVBoxLayout *layOut;
    //声音滑块布局
    QHBoxLayout *volumeLayout;
    QLabel *volumeLabel;
    QSlider *volumeSlider;
    //暂停和继续
    QPushButton *PauseContinue;
    //返回上一级
    QPushButton *Back;

public:
    explicit SettingsMenu(QWidget *parent = nullptr);

signals:
    void volumeChanged(int volume);
    void GamePause();
    void GameContinue();
    void getBack();


};

#endif // SETTINGSMENU_H
