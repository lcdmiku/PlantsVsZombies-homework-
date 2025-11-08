#include "settingsmenu.h"

SettingsMenu::SettingsMenu(QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle("Settings");


    layOut = new QVBoxLayout(this);
    //volume
    volumeLayout = new QHBoxLayout(this);
    volumeLabel = new QLabel("volume:50",this);
    volumeSlider = new QSlider(Qt::Horizontal,this);
    volumeSlider->setRange(0,100);
    volumeSlider->setValue(50);
    connect(volumeSlider,&QSlider::valueChanged,this,[=](int value){
        volumeLabel->setText(QString("volume:%1").arg(value));
        emit volumeChanged(value);
    });

    volumeLayout->addWidget(volumeLabel);
    volumeLayout->addWidget(volumeSlider);
    //
    layOut->addLayout(volumeLayout);
    //pause and restart
    PauseContinue = new QPushButton("Pause",this);
    connect(PauseContinue,&QPushButton::clicked,PauseContinue,[=](){
        if(PauseContinue->text() == "Pause")
        {
            PauseContinue->setText("Continue");
            emit GamePause();
        }
        else{
            PauseContinue->setText("Pause");
            emit GameContinue();
        }
    });
    layOut->addWidget(PauseContinue);
    //
    //返回上一级
    Back = new QPushButton("Back",this);
    connect(Back,&QPushButton::clicked,this,&SettingsMenu::getBack);
    layOut->addWidget(Back);


}
