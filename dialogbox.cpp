#include "dialogbox.h"
#include<QGraphicsSceneMouseEvent>

DialogBox::DialogBox(QGraphicsObject *parent) : QGraphicsWidget(parent),
    remain(25),w(350),h(120),borderWidth(30)
{
    // 加载边框图片（提前准备好边框资源）
    if(parent)
        setPos(-w/4.0,-(h+remain)-parent->boundingRect().height()/2);
    setMinimumSize(w, h+remain);  // 最小尺寸，轻量化控制
    PixInit();
    // this->hide();

}
//borderInit has buttonPix Init
void DialogBox::PixInit(){
    //
    bottomleft.load(":/res/GameRes/images/interface/dialog_bottomleft_thin.png");
    bottommiddle.load(":/res/GameRes/images/interface/dialog_bottommiddle_thin.png");
    bottomright.load(":/res/GameRes/images/interface/dialog_bottomright_thin.png");
    //
    centerleft.load(":/res/GameRes/images/interface/dialog_centerleft.png");
    centermiddle.load(":/res/GameRes/images/interface/dialog_centermiddle.png");
    centerright.load(":/res/GameRes/images/interface/dialog_centerright.png");
    //
    topleft.load(":/res/GameRes/images/interface/dialog_topleft.png");
    topmiddle.load(":/res/GameRes/images/interface/dialog_topmiddle.png");
    topright.load(":/res/GameRes/images/interface/dialog_topright.png");
    //
    header.load(":/res/GameRes/images/interface/dialog_header.png");
    //
    buttonPix.load(":/res/GameRes/images/interface/Button.png");
    //
    linePix.load(":/res/GameRes/images/interface/dialog_line.png");

}
void DialogBox::btnsGenerate(const QVector<QString>& btnStrs){
    //
    m_branchBtns.clear();
    for(int i=0;i<btnStrs.size();i++){
        m_branchBtns.push_back(BranchBtn(btnStrs[i],i));
    }
}
QVector<QPointF> DialogBox::getTrianglePoints(){
    QVector<QPointF> triangle;
    QPointF p0(w/4.0,h-10);
    QPointF p1(w/4.0+30,h-10);
    QPointF p2(w/4.0,h+remain);
    triangle.push_back(p0);
    triangle.push_back(p1);
    triangle.push_back(p2);
    return triangle;
}
void DialogBox::DrawLine(QPainter *painter,QPointF& p1,QPointF& p2,QPixmap& pix){
    painter->save();
    painter->translate(p2.x(), p2.y());
    QLineF line = QLineF(p1,p2);
    qreal length = line.length();
    qreal width = 5;
    painter->rotate(-line.angle()+90);
    painter->drawPixmap(0,0,width,length,pix);//先使用button的图片将就一下
    painter->restore();
}
void DialogBox::setDialog(QString text, const QVector<QString>& btnStrs,QString avatarPath) {
    if(!avatarPath.isEmpty())
    m_avatarPixmap.load(avatarPath);
    m_dialogText = text;
    btnsGenerate(btnStrs);
    update();  // 刷新绘制
}

void DialogBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option); Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing);  // 抗锯齿，轻量化美化

    //绘制边框
    QRectF widgetRect = rect();
    //边框离widget底部预留宽度,同时也是按键高度

    // 绘制四角
    painter->drawPixmap(0, 0, borderWidth, borderWidth, topleft);
    painter->drawPixmap(widgetRect.width()-borderWidth, 0, borderWidth, borderWidth,topright);
    painter->drawPixmap(0, widgetRect.height()-borderWidth -remain, borderWidth, borderWidth, bottomleft);//re
    painter->drawPixmap(widgetRect.width()-borderWidth, widgetRect.height()-borderWidth -remain, borderWidth, borderWidth, bottomright);//re
    // 绘制上下边
    painter->drawPixmap(borderWidth, 0, widgetRect.width()-2*borderWidth, borderWidth, topmiddle);
    painter->drawPixmap(borderWidth, widgetRect.height()-borderWidth -remain, widgetRect.width()-2*borderWidth, borderWidth,bottommiddle);//re
    // 绘制左右边和中间背景
    painter->drawPixmap(0, borderWidth, borderWidth, widgetRect.height()-2*borderWidth-remain, centerleft);//re
    painter->drawPixmap(widgetRect.width()-borderWidth, borderWidth, borderWidth, widgetRect.height()-2*borderWidth-remain, centerright);//re
    painter->drawPixmap(borderWidth, borderWidth, widgetRect.width()-2*borderWidth, widgetRect.height()-2*borderWidth-remain, centermiddle);//re
    //绘制尖角
    painter->setBrush(centermiddle.isNull() ? QColor(220, 220, 220) : centermiddle.toImage().pixelColor(0,0));
    QVector<QPointF> triangle = getTrianglePoints();
    painter->setPen(Qt::NoPen);
    painter->drawConvexPolygon(triangle);
    DrawLine(painter,triangle[0],triangle[2],linePix);
    DrawLine(painter,triangle[1],triangle[2],linePix);

    // 绘制头像或者立绘
    if(!m_avatarPixmap.isNull())
    painter->drawPixmap(borderWidth+10, borderWidth+10, 40, 40, m_avatarPixmap.scaled(40,40, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    //绘制对话文本
    QRectF textRect = QRectF(borderWidth+60, borderWidth+10, widgetRect.width()-80, 80);
    painter->setPen(Qt::white);
    painter->setFont(QFont("微软雅黑", 15));
    painter->drawText(textRect, Qt::TextWordWrap, m_dialogText);

    // 绘制互动按键（底部）
    int btnY = widgetRect.height() - remain;//按键放在预留处
    int btnWidth = 80;
    int btnHeight = remain;
    int totalWidth = 0;
    for (int i=0; i<m_branchBtns.size(); i++) {
        BranchBtn &btn = m_branchBtns[i];
        btnWidth = 15 * btn.text.size() + 10;
        totalWidth += btnWidth;
        btn.rect = QRectF(w-totalWidth, btnY, btnWidth, btnHeight);
        // 按键背景
        painter->drawPixmap(btn.rect.x(),btn.rect.y(),
                            btn.rect.width(),btn.rect.height(),
                            buttonPix);

        // 按键文本
        painter->setPen(Qt::yellow);
        painter->setFont(QFont("微软雅黑", 15));
        painter->drawText(btn.rect, Qt::AlignCenter, btn.text);
    }
}
void DialogBox::resizeEvent(QGraphicsSceneResizeEvent *event){

}
void DialogBox::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    // 检测点击是否命中按键
    for (auto &btn : m_branchBtns) {
        if (btn.rect.contains(event->pos())) {
            emit branchTriggered(btn.branchId);
            qDebug()<<"clicked"<<btn.branchId<<"button";
            break;
        }
    }
    QGraphicsWidget::mousePressEvent(event);
}
