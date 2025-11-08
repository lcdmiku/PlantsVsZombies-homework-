#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include<QPushButton>
#include <QWidget>

class StartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StartWidget(QWidget *parent = nullptr);
    QPushButton *start_btn;


signals:
    void startGame();
};

#endif // STARTWIDGET_H
