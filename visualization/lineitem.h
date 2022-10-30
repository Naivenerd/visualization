#ifndef LINEITEM_H
#define LINEITEM_H

#include <QGraphicsItem>
#include <QtMath>
#include <QPen>

class LineItem  : public QGraphicsLineItem
{
public:
    LineItem(QPointF ori, QPointF dst, QGraphicsItem *parent = nullptr);

    enum ItemType{
        GraphicsItem_line = UserType + 3,
    };

    QGraphicsPolygonItem *arrow = nullptr;  //箭头

    void update_ori(QPointF ori);
    void update_dst(QPointF dst);
    void update_line(QLineF line);

protected:
    //虚函数的重新实现
    int type() const override;

private:
    qreal line_width = 5;
    qreal arrow_radius = 6;
    QColor color = Qt::magenta;

    //刷新位置
    QPointF ori;
    QPointF dst;
    void draw();
};

#endif // LINEITEM_H
