#ifndef MYLISTVIEW_H
#define MYLISTVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

#include "dataitem.h"

class MyListView : public QGraphicsView
{
    Q_OBJECT
public:
    MyListView(QWidget *parent = nullptr);

    QGraphicsScene *myGraphicsScene;

    int vexNum = 0;
    int arcNum = 0;

    void SaveToFile(QString path);
    bool night_mode = false;
    void setTheme(bool night_mode);

    QList<DataItem *> data_items;
    int next_value = 0;
    DataItem *create_item(QPoint point, int value);

    enum Target_state{
        Target_normal,
        Target_focus,
        Target_just_focus_out,
        Target_active
    }target_state = Target_normal;
signals:
    void selected(QGraphicsItem *item);
    void unselected();
    void info_changed();
    void log_signal(QString log_info);

signals:
    void debug_signal(QString msg);

protected slots:
    void actived_slot();

protected:
    void mouseReleaseEvent(QMouseEvent *event);
};



#endif // MYLISTVIEW_H
