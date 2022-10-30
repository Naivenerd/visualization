#include "dataitem.h"

DataItem::DataItem(QPointF point, int value, QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    bound = new QGraphicsEllipseItem(this);
    bound->setRect(QRectF(-diameter/2, -diameter/2, diameter/2, diameter/2));
    bound->setBrush(brush_bound_default);
    bound->setPen(QPen(QBrush(), 0));
    bound->setPos(0, 0);

    text = new QGraphicsTextItem(this);
    //text->setTextInteractionFlags(Qt::TextEditable);
    text->setPlainText(QString::number(value));
    text->setTextWidth(diameter);
    text->setFont(font);
    text->setTextWidth(100);
    text->setPos(0, -20);

    setPos(point);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsFocusable);
}

void DataItem::set_data(int data)
{
    text->setPlainText(QString::number(data));
}

void DataItem::set_editable(bool enable)
{
    if(enable)
        text->setTextInteractionFlags(Qt::TextEditable);
    else
        text->setTextInteractionFlags(Qt::NoTextInteraction);
}

int DataItem::get_data()
{
    return text->toPlainText().toInt();
}

QRectF DataItem::boundingRect() const
{
    return bound->boundingRect();
}

void DataItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void DataItem::focusInEvent(QFocusEvent *event)
{
    state = State_focus;
    update();
    QGraphicsItem::focusInEvent(event);
}

void DataItem::focusOutEvent(QFocusEvent *event)
{
    state = State_default;
    mouse_release_time = 0;
    update();
    QGraphicsItem::focusOutEvent(event);
}

void DataItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(active_enable && ++mouse_release_time > 1)
    {
        state = State_picked;
        emit actived();
    }
    update();
    QGraphicsItem::mouseReleaseEvent(mouseEvent);
}

void DataItem::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    update();
    QGraphicsItem::mouseMoveEvent(mouseEvent);
}

int DataItem::type() const
{
    return GraphicsItem_data;
}

void DataItem::set_visited(bool visited)
{
    if(visited)
        bound->setBrush(brush_bound_visited);
    else
        bound->setBrush(brush_bound_default);
    QGraphicsItem::update();
}

void DataItem::set_active_enable(bool enable)
{
    active_enable = enable;
}

void DataItem::update()
{
    bound->setBrush(brush_bound_default);
    switch (state) {
    case State_default: bound->setBrush(brush_bound_default); break;
    case State_focus: bound->setBrush(brush_bound_focus); break;
    case State_picked: bound->setBrush(brush_bound_picked); break;
    default:
        break;
    }

    for(int i = 0; i < lines_shot.length(); i++)
    {
        lines_shot[i]->update_line(get_interspace_line(this, line_dsts[i]));
    }
    for(int i = 0; i < lines_receive.length(); i++)
    {
        lines_receive[i]->update_line(get_interspace_line(line_srcs[i], this));
    }
    QGraphicsItem::update();
}

LineItem *DataItem::connect_item(DataItem *src, DataItem *dst)
{
    if(!(src && dst))
        return nullptr;
    if(src->line_dsts.contains(dst))
    {
        int index_src = src->line_dsts.indexOf(dst);
        return src->lines_shot[index_src];
    }
    QLineF line_pos = get_interspace_line(src, dst);
    LineItem *line = new LineItem(line_pos.p1(), line_pos.p2());

    src->lines_shot.append(line);
    src->line_dsts.append(dst);
    dst->lines_receive.append(line);
    dst->line_srcs.append(src);

    src->scene()->addItem(line);
    return line;
}

bool DataItem::disconnect_item(DataItem *src, DataItem *dst)
{
    if(!(src && dst))
        return false;
    if(src->line_dsts.contains(dst))
    {
        int index_src = src->line_dsts.indexOf(dst);
        int index_dst = dst->line_srcs.indexOf(src);
        LineItem *line = src->lines_shot[index_src];
        src->lines_shot.removeAt(index_src);
        src->line_dsts.removeAt(index_src);
        dst->lines_receive.removeAt(index_dst);
        dst->line_srcs.removeAt(index_dst);
        line->scene()->removeItem(line);
        delete line;
        return true;
    }
    else
    {
        return false;
    }
}

void DataItem::clear_shot()
{
    for(int i = lines_shot.length() - 1; i >= 0; i--)
    {
        disconnect_item(this, line_dsts[i]);
    }
}

void DataItem::clear_receive()
{
    for(int i = lines_receive.length() - 1; i >= 0; i--)
    {
        disconnect_item(line_srcs[i], this);
    }
}

QLineF DataItem::get_interspace_line(DataItem *src, DataItem *dst)
{
    QPointF src_center = src->pos() + QPointF(src->diameter/2, src->diameter/2);
    QPointF dst_center = dst->pos() + QPointF(dst->diameter/2, dst->diameter/2);
    QPointF diff = dst_center - src_center;
    qreal distance = qSqrt(diff.x() * diff.x() + diff.y() * diff.y());
    QPointF intersection_src = diff * (src->diameter/2*1.2) / distance; //起点圆周交点与圆心的偏移量
    QPointF intersection_dst = -diff * (dst->diameter/2*1.2) / distance; //终点点圆周交点与圆心的偏移量
    return QLineF(src_center + intersection_src,
                  dst_center + intersection_dst);
}

QString DataItem::export_string()
{
    stringstream stream;
    stream << get_data() << " "
           << x() << " "
           << y();
    return QString::fromStdString(stream.str());
}

void DataItem::import_string(QString info)
{
    stringstream stream(info.toStdString());
    int data;
    double pos_x, pos_y;
    stream >> data >> pos_x >> pos_y;
    set_data(data);
    setPos(pos_x, pos_y);
    update();
}

void DataItem::setTheme(bool night_mode)
{
    if(night_mode)
    {
        brush_bound_default.setColor(Qt::white);
        text->setDefaultTextColor(Qt::white);
    }
    else
    {
        brush_bound_default.setColor(Qt::blue);
        text->setDefaultTextColor(Qt::black);
    }
    update();
}

