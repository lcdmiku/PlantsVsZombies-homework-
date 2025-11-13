#ifndef DIALOGBOX_H
#define DIALOGBOX_H

#include <QGraphicsWidget>
#include <QPainter>
#include <QVector>

class DialogBox : public QGraphicsWidget {
    Q_OBJECT
public:
    struct BranchBtn {
        QString text;  // 按键文本
        QRectF rect;
        int branchId ; // 按键区域
        BranchBtn(QString text,int id):text(text),branchId(id){}
    };

    explicit DialogBox(QGraphicsObject *parent = nullptr);
    void setDialog( QString text, const QVector<QString>& btnStrs = QVector<QString>(),QString avatarPath="");  // 设置对话内容,默认立绘为空,外部输入btnstr，内部自处理

signals:
    void branchTriggered(int BtnId);  // 分支触发信号

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void resizeEvent(QGraphicsSceneResizeEvent *event) override;

private:
// 边框图片
    QPixmap   bottomleft;
    QPixmap   bottommiddle;
    QPixmap   bottomright;
    QPixmap   centerleft;
    QPixmap   centermiddle;
    QPixmap   centerright;
    QPixmap   topleft;
    QPixmap   topmiddle;
    QPixmap   topright;
    QPixmap   header;
    QPixmap   linePix;
//边框宽度
    int borderWidth;
//按键预留空间
    int remain;
//
//实际按键空间
    int w;
    int h;
//边框指向角色箭头处理
    QVector<QPointF> getTrianglePoints();
    void DrawLine(QPainter *painter,QPointF& p1,QPointF& p2,QPixmap& pix);//画有图片样式的粗线条

//按键图片
    QPixmap buttonPix;
// 角色头像
    QPixmap m_avatarPixmap;
    void PixInit();

    QString m_dialogText;    // 对话文本
    QVector<BranchBtn> m_branchBtns;  // 互动按键
    void btnsGenerate(const QVector<QString>& btnStrs);
};


#endif // DIALOGBOX_H
