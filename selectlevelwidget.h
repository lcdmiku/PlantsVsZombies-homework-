#ifndef SELECTLEVELWIDGET_H
#define SELECTLEVELWIDGET_H

#include <QWidget>
#include <QPushButton>
#include<QSettings>

class SelectLevelWidget : public QWidget
{
    Q_OBJECT
    int currentNum;//现在显示关卡数量
public:
    explicit SelectLevelWidget(QWidget *parent = nullptr);
    void addLevelButton(const QString& iconPath, const QString& levelName);
    void addLevelButton();
signals:
    void levelSelected(int num); //发送选择关卡信号

private:
    //传入图片和关卡名，未来要绑定关卡再加参数。
    void addLevelButton(int x, int y, const QString& iconPath, const QString& levelName);

};

#endif // SELECTLEVELWIDGET_H
