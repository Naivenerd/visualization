#ifndef DATAITEM_H
#define DATAITEM_H

#include <QGraphicsItem>
#include <QTextBlockFormat>
#include <QTextCursor>
#include <QtDebug>
#include <QGraphicsScene>

#include <sstream>
using namespace std;

#include "lineitem.h"

class DataItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    DataItem(QPointF point, int value = 0,QGraphicsItem *parent = nullptr);

    //设置数值
    virtual void set_data(int data);
    virtual int get_data();
    void set_editable(bool enable);

    //更新显示（颜色、箭头等）
    void update();
    void clear_shot();
    void clear_receive();

    static LineItem *connect_item(DataItem *src, DataItem *dst);
    static bool disconnect_item(DataItem *src, DataItem *dst);

    enum ItemType{
        GraphicsItem_data = UserType + 1,
    };
    virtual int type() const override;

    //导入导出字符串
    virtual QString export_string();
    virtual void import_string(QString info);

    QList<LineItem *> lines_shot;       //从此点发出的连接线
    QList<LineItem *> lines_receive;    //射向此点的连接线
    QList<DataItem *> line_dsts;         //此点指向的数据点
    QList<DataItem *> line_srcs;         //指向此点的数据点

    qreal diameter = 20;
    QBrush brush_bound_default = QBrush(Qt::blue);
    QBrush brush_bound_focus = QBrush(Qt::green);
    QBrush brush_bound_picked = QBrush(Qt::red);
    QBrush brush_bound_visited = QBrush(Qt::red);
    QFont font = QFont("宋体", 20);
    bool active_enable = true;
    void set_active_enable(bool enable); //设置是否开启可激活状态（即可连线状态）
    void set_visited(bool visited);

    void setTheme(bool night_mode);

private:
    static QLineF get_interspace_line(DataItem *src, DataItem *dst);

    enum State{
        State_default,
        State_focus,
        State_picked
    }state = State_default;

    QGraphicsEllipseItem *bound = nullptr;       //圆形边界
    QGraphicsTextItem *text = nullptr;          //数据域

protected:
    //虚函数的重新实现
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    int mouse_release_time = 0;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

signals:
    void actived();
    void debug_signal(QString msg);
};

#endif // DATAITEM_H
