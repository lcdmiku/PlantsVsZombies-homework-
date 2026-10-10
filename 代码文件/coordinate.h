#ifndef COORDINATE_H
#define COORDINATE_H

#include <QObject>
#include <algorithm>
#include <QtGlobal>
#include <QDebug>

int truncBetween(int value, int low, int high); //将数据范围限制在low和high之间。

class Coordinate
{
public:
    Coordinate(int coord = 0);
    int getCol(qreal x) const; //视图x值->列序号
    int getRow(qreal y) const; //视图y值->行序号
    double getX(int c) const;
    double getY(int r) const;
    QPair<qreal, int> choosePlantX(qreal x) const;
    QPair<qreal, int> choosePlantY(qreal y) const;

    int rowCount() const;
    int colCount() const;

private:
    int row, col; //行，列
    QPair<QList<qreal>, QList<int> > x2c, y2r; //视图坐标->行列序号的转换表格
    QPair<QList<int>, QList<qreal> > c2x, r2y; //行列序号->视图坐标的转换表格
};

#endif // COORDINATE_H
