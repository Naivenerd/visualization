#include "lineitem.h"

LineItem::LineItem(QPointF ori, QPointF dst, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    this->ori = ori;
    this->dst = dst;

    //初始化线段
    QPen pen_line(QBrush(Qt::magenta), line_width);
    setPen(pen_line);
    setPos(ori);
    setLine(QLineF(QPointF(0, 0), dst - ori));

    //初始化箭头
    arrow = new QGraphicsPolygonItem(this);
    arrow->setPos(dst - ori);
    //设置三角形的3个顶点
    QPolygonF polygon;
    polygon << QPointF(0,-arrow_radius) << QPointF(0, arrow_radius) << QPointF(arrow_radius, 0);
    arrow->setPolygon(polygon);
    //设置三角形的边
    pen_line.setWidth(1);
    arrow->setPen(pen_line);
    //设置三角形的内部
    QBrush arrow_brush(Qt::magenta);
    arrow_brush.setStyle(Qt::SolidPattern);
    arrow->setBrush(arrow_brush);

    draw();
}

int LineItem::type() const
{
    return GraphicsItem_line;
}

void LineItem::draw()
{
    setPos(ori);
    setLine(QLineF(QPointF(0, 0), dst - ori));

    QPointF line_dst = dst - ori;
    arrow->setPos(line_dst);
    qreal angel = qAtan2(line_dst.y(), line_dst.x());
    qreal degree = angel / M_PI / 2 * 360;
    arrow->setRotation(degree);
    update();
}

void LineItem::update_ori(QPointF ori)
{
    this->ori = ori;
    draw();
}

void LineItem::update_dst(QPointF dst)
{
    this->dst = dst;
    draw();
}

void LineItem::update_line(QLineF line)
{
    this->ori = line.p1();
    this->dst = line.p2();
    draw();
}

