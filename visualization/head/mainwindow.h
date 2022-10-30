#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QGraphicsDropShadowEffect>
#include <QDateTime>
#include "slidepage.h"
#include "mylistcanvas.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    int cornerRadius = 50;
    QWidget *border = nullptr;
    QWidget *defaultPage;
    QGraphicsDropShadowEffect *windowShadow;
    QColor mainBackGround = QColor(251, 251, 251);

    QLineEdit *canvasTitle = nullptr;
    QLineEdit *canvasDesc = nullptr;
    customIcon *settingsIcon = nullptr;
    customIcon *themeIcon = nullptr;
    customIcon *layersIcon = nullptr;
    QWidget *canvasDisplay = nullptr;

    QVector<SlidePage *> pageList;
    SlidePage *defaultSettingsPage = nullptr;
    SlidePage *curSettingsPage = nullptr;
    singleSelectGroup *layerSel = nullptr;

    MyListCanvas *my_list_canvas = nullptr;
    MyListCanvas *curCanvas = nullptr;

    void selectListCanvas(MyListCanvas *canvas);
    void deleteListCanvas(MyListCanvas *canvas);
    void Init();

    enum
    {
        AT_LEFT = 1,
        AT_TOP = 2,
        AT_RIGHT = 4,
        AT_BOTTOM = 8,
        AT_TOP_LEFT = 3,
        AT_TOP_RIGHT = 6,
        AT_BOTTOM_LEFT = 9,
        AT_BOTTOM_RIGHT = 12
    };
    bool mousePressed = false;
    int mouseState;
    QPoint lastPos;
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event)
    {
        mousePressed = false;
        if (event->globalPosition().y() < 2)
            controlWindowScale();
    }
    void resizeEvent(QResizeEvent *event);

    bool maximized = false;
    QRect lastGeometry;
    void controlWindowScale();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    enum Theme{
        Theme_write = 0,
        Theme_black = 1,
        Theme_none = 2
    };
    Theme theme = Theme_write;
    bool night_mode = false;
    bigIconButton *Linked_List = nullptr;
    bigIconButton *Queue = nullptr;
    bigIconButton *BST = nullptr;

    void theme_switch();
    void theme_refresh();
};
#endif // MAINWINDOW_H
