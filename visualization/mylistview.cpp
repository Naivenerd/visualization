#include "mylistview.h"

MyListView::MyListView(QWidget *parent)
    : QGraphicsView(parent)
{
    myGraphicsScene = new QGraphicsScene();
    this->setScene(myGraphicsScene);

    this->setMouseTracking(true);
    this->setBackgroundBrush(Qt::transparent);
    this->setRenderHint(QPainter::Antialiasing);
    this->setCursor(Qt::CrossCursor);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
}

void MyListView::SaveToFile(QString path)
{
    Q_UNUSED(path);
}

void MyListView::setTheme(bool night_mode)
{
    this->night_mode = night_mode;
    QColor background_color = Qt::white;
    QColor content_color = Qt::black;
    if(night_mode)
    {
        background_color = Qt::black;
        content_color = Qt::white;
    }
    setStyleSheet(QString("background-color:%1;color:%2;")
                  .arg(background_color.name())
                  .arg(content_color.name()));
    foreach (DataItem *item, data_items) {
        item->setTheme(night_mode);
    }
}

void MyListView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsItem *it_mouse = itemAt(event->pos());
    //若点击处什么也没有（或点击处是一个文本），则在点击处创建一个新的
    if((it_mouse == nullptr || it_mouse->type() == QGraphicsTextItem::Type)
            && target_state == Target_normal)
    {
        create_item(event->pos(), next_value++);
    }
    if(target_state == Target_just_focus_out)
        target_state = Target_normal;
    QGraphicsView::mouseReleaseEvent(event);
}

DataItem *MyListView::create_item(QPoint point, int value)
{
    DataItem *it = new DataItem(mapToScene(point), value);
    it->setTheme(night_mode);
    myGraphicsScene->addItem(it);
    data_items.append(it);
    connect(it, SIGNAL(actived()),
            this, SLOT(actived_slot()));
    connect(it, SIGNAL(debug_signal(QString)),
            this, SIGNAL(debug_signal(QString)));
    emit debug_signal(QString("创建了一个值为 %1 的数据点").arg(value));
    return it;
}

void MyListView::actived_slot()
{

}
