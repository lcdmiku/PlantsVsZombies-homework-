#include "coordinate.h"

Coordinate::Coordinate(int coord) : col(9)
{
    //土地左上角点(255,90),而width = 81, height = 94
    x2c = QPair<QList<qreal>, QList<int> >(
    // { -50.0, 100.0, 140.0, 220.0, 295.0, 379.0, 460.0, 540.0, 625.0, 695.0, 775.0, 855.0, 935.0 },
    // {  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11 });
        {255, 336, 417, 498, 579, 660, 741, 822, 903,  984, 1065, 1146, 1227 },  // 阈值：每列左边界（255 + 81*n）
        { -1,   0,   1,   2,   3,   4,   5,   6,   7,    8,    9,   10,   11});  // 列索引：从0开始索引

    c2x = QPair<QList<int>, QList<qreal> >(
        // {  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11 },
        // { -50, 100, 140, 187, 267, 347, 427, 507, 587, 667, 747, 827, 865, 950 });
        {    0,     1,     2,     3,     4,     5,     6,     7,     8,      9,     10,     11}, //列数
        {295.5, 376.5, 457.5, 538.5, 619.5, 700.5, 781.5, 862.5, 943.5, 1024.5, 1105.5, 1186.5}); //返回格子中心的x坐标
    if (coord == 0) {
        row = 5;
        y2r = QPair<QList<qreal>, QList<int> >(
            // {  86, 181, 281, 386, 476 },
            // {   0,   1,   2,   3,   4,   5 });
            {90, 184, 278, 372, 466, 560, 654},  // 阈值：每行上边界（90 + 94*n）
            {-1, 0, 1, 2, 3, 4, 5, 6});        // 行索引：y <90→-1；90≤y<184→0；...；≥466→4
        r2y = QPair<QList<int>, QList<qreal> >(
            // {   0,   1,   2,   3,   4,   5 },
            // {  75, 175, 270, 380, 470, 575 });
            {0, 1, 2, 3, 4},
            {137, 231, 325, 419, 513});
    }
    else { // Pool
        row = 6;
        y2r = QPair<QList<qreal>, QList<int> >(
            // {  86, 171, 264, 368, 440, 532 },
            // {   0,   1,   2,   3,   4,   5,  6 });
            {90, 184, 278, 372, 466, 560},  // 阈值：90 + 94*n（n=0~5）
            {-1, 0, 1, 2, 3, 4, 5});          // 行索引：0~5
        r2y = QPair<QList<int>, QList<qreal> >(
            // {   0,   1,   2,   3,   4,   5,   6 },
            // {  75, 165, 253, 355, 430, 552, 587 });
            {0, 1, 2, 3, 4, 5},
            {137, 231, 325, 419, 513, 607});
    }
}

int Coordinate::getCol(qreal x) const
{
    //todo
    auto it = std::lower_bound(x2c.first.begin(),x2c.first.end(), x);
    if(it == x2c.first.end())return -1;
    int i = x2c.second[it - x2c.first.begin()]; //找到最后一个比x小的边界并返回其边界
    return (i >= 0 && i < col ) ? i : -1; //超出地面的一律返回-1
}

int Coordinate::getRow(qreal y) const
{
     //todo
    auto it = std::lower_bound(y2r.first.begin(),y2r.first.end(), y);
    if(it == y2r.first.end())return -1;
    int i = y2r.second[it - y2r.first.begin()]; //同上
    return (i >= 0 && i < row ) ? i : -1; //同上
}

qreal Coordinate::getX(int c) const
{
     //todo
    if(c < 0 || c >= col)return -1; //超出边界的返回-1
    auto it = std::find(c2x.first.begin(),c2x.first.end(), c);
    if(it == c2x.first.end())return -1;
    return c2x.second[it - c2x.first.begin()];
}

qreal Coordinate::getY(int r) const
{
     //todo
    if(r < 0 || r >= row)return -1;
    auto it = std::find(r2y.first.begin(),r2y.first.end(), r);
    if(it == r2y.first.end())return -1;
    return r2y.second[it - r2y.first.begin()];
}

QPair<qreal, int> Coordinate::choosePlantX(qreal x) const
{
    int c = getCol(x);
    return QPair<qreal, int>(getX(c), c);
}

QPair<qreal, int> Coordinate::choosePlantY(qreal y) const
{
    int r = getRow(y);
    return QPair<qreal, int>(getY(r), r);
}

int truncBetween(int value, int low, int high)
{
    if (value < low)
        return low;
    if (value > high)
        return  high;
    return value;
}

int Coordinate::rowCount() const
{
    return row;
}

int Coordinate::colCount() const
{
    return col;
}
