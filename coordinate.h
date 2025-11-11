#ifndef COORDINATE_H
#define COORDINATE_H

#include <QObject>

int truncBetween(int value, int low, int high);

class Coordinate
{
public:
    Coordinate(int coord = 0);
    int getCol(double x) const;
    int getRow(double y) const;
    double getX(int c) const;
    double getY(int r) const;
    QPair<double, int> choosePlantX(double x) const;
    QPair<double, int> choosePlantY(double y) const;

    int rowCount() const;
    int colCount() const;

private:
    int row, col;
    QPair<QList<double>, QList<int> > x2c, y2r;
    QPair<QList<int>, QList<double> > c2x, r2y;
};

#endif // COORDINATE_H
