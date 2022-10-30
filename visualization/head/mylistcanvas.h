#ifndef MYLISTCANVAS_H
#define MYLISTCANVAS_H

#include <QWidget>
#include <QGridLayout>
#include "slidepage.h"
#include "mylistview.h"
#include "graph_view.h"

class MyListCanvas : public QWidget
{
    Q_OBJECT
private:
    QString canvasName;
    QString canvasDescription;

    SlidePage *settings;

    //For display
    MyListView *view;
    QHBoxLayout *mainLayout;
    QWidget *infoWidget;
    QLabel *pageName;
    QLabel *logLabel;

    //info区的部件
    QVector<textInputItem *> text_input_items;
    QVector<textButton *> text_buttons;

    void CreateSettings(int r);
    void Init();

public:
    explicit MyListCanvas(int radius, QString name = "", QString desc = "", QWidget *parent = nullptr);
    QString name(){return canvasName;}
    QString description(){return canvasDescription;}
    SlidePage *settingPage(){return settings;}
    bool night_mode = 0;
    void setTheme(bool night_mode);

signals:
    void nameChanged(QString name);
    void descChanged(QString desc);
    void setDel(MyListCanvas* target);
};

#endif // MYLISTCANVAS_H
