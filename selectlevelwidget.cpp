#include "selectlevelwidget.h"
#include <QPainter>
#include <QPixmap>
#include <QLabel>
#include <QDebug>

SelectLevelWidget::SelectLevelWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(900, 600);
    setAutoFillBackground(true);
    
    // 设置背景
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(QPixmap(":/res/GameRes/images/interface/Challenge_Background.jpg")
                                                  .scaled(size(),
                                                          Qt::IgnoreAspectRatio,
                                                          Qt::SmoothTransformation)));
    setPalette(palette);

    // 创建关卡选择按钮
    addLevelButton(50, 100, ":/res/GameRes/images/interface/baolingqiu.png", "关卡1");
    //创建多个关卡例子
    // addLevelButton(200, 100, ":/res/GameRes/images/interface/baolingqiu.png", "关卡2");
}

void SelectLevelWidget::addLevelButton(int x, int y, const QString& iconPath, const QString& levelName)
{
    // 创建关卡选择按钮 (使用 Challenge.png 作为背景)
    QPushButton *btn = new QPushButton(this);
    QPixmap btnPix(":/res/GameRes/images/interface/Challenge.png");
    
    btn->resize(btnPix.size());
    btn->move(x, y); // 设置位置
    
    // 设置按钮样式
    btn->setStyleSheet("QPushButton{border-image: url(:/res/GameRes/images/interface/Challenge.png); border:none;}"
                            "QPushButton:pressed{margin-top: 2px; margin-left: 2px;}");

    // 在按钮上放置图标
    QLabel *iconLabel = new QLabel(btn);
    QPixmap iconPix(iconPath);
    iconLabel->setPixmap(iconPix);
    iconLabel->setAttribute(Qt::WA_TransparentForMouseEvents); // 让点击事件穿透到按钮
    
    // 居中显示图标
    int iconX = (btn->width() - iconPix.width()) / 2;
    int iconY = (btn->height() - iconPix.height()) / 10 + 1;
    iconLabel->move(iconX, iconY);

    // 添加关卡名称标签
    QLabel *nameLabel = new QLabel(levelName, this);
    // 设置字体样式：黑色粗体，带阴影效果模拟游戏风格
    nameLabel->setStyleSheet("QLabel { color: black; font-family: 'Arial'; font-size: 14px; font-weight: bold; }");
    nameLabel->adjustSize(); // 自适应大小

    // 计算位置：水平居中于按钮，垂直位于按钮下方
    int labelX = x + (btn->width() - nameLabel->width()) / 2;
    int labelY = y + btn->height() - 35; // 间距调出来的，喜欢继续
    nameLabel->move(labelX, labelY);

    connect(btn, &QPushButton::clicked, this, &SelectLevelWidget::levelSelected);
}
