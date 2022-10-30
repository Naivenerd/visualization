#include "./head/mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainterPath>
#include <QRegion>
#include <QTimer>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setMouseTracking(true);
    QTimer *t = new QTimer(this);
    connect(t, &QTimer::timeout, this, [=]()
            { Init();
        theme_refresh(); });
    t->setSingleShot(true);
    t->start(10);

    connect(ui->adjSizeBtn, &QPushButton::clicked, this, [=]()
            { controlWindowScale();});
}

void MainWindow::Init()
{
    /* Create main widget and set mask, style sheet and shadow */
    QPainterPath path;
    path.addRoundedRect(ui->mainWidget->rect(), cornerRadius, cornerRadius);
    QRegion mask(path.toFillPolygon().toPolygon());
    ui->mainWidget->setMask(mask);

    QString mainStyle;
    ui->mainWidget->setObjectName("mainWidget");
    mainStyle = "QWidget#mainWidget{background-color:" + mainBackGround.name() + QString::asprintf(";border-radius:%dpx", cornerRadius) + "}";
    ui->mainWidget->setStyleSheet(mainStyle);

    windowShadow = new QGraphicsDropShadowEffect(this);
    windowShadow->setBlurRadius(30);
    windowShadow->setColor(QColor(0, 0, 0));
    windowShadow->setOffset(0, 0);
    ui->mainWidget->setGraphicsEffect(windowShadow);
    /**********************************************************/

    /* Create border in order to cover the zigzag edge of the region */
    border = new QWidget(this);
    border->move(ui->mainWidget->pos() - QPoint(1, 1));
    border->resize(ui->mainWidget->size() + QSize(2, 2));
    QString borderStyle;
    borderStyle = "background-color:#00FFFFFF;border:1.5px solid #686868; border-radius:" + QString::asprintf("%d", cornerRadius) + "px";
    border->setStyleSheet(borderStyle);
    border->setAttribute(Qt::WA_TransparentForMouseEvents);
    border->show();
    /*****************************************************************/

    /* Create about page */
    defaultSettingsPage = new SlidePage(cornerRadius, "ABOUT", ui->mainWidget);
    textInputItem *version = new textInputItem("version", defaultSettingsPage);
    version->setValue("1.3-beta");
    version->setEnabled(false);
    textInputItem *updateDate = new textInputItem("last-upd", defaultSettingsPage);
    updateDate->setValue("2021/12/6 10:14");
    updateDate->setEnabled(false);
    textInputItem *Author = new textInputItem("author", defaultSettingsPage);
    Author->setValue("Linloir | Made with love");
    Author->setEnabled(false);
    textInputItem *lic = new textInputItem("lic", defaultSettingsPage);
    lic->setValue("MIT License");
    lic->setEnabled(false);
    textInputItem *GitHub = new textInputItem("git", defaultSettingsPage);
    GitHub->setValue("github.com/Linloir");
    GitHub->setEnabled(false);
    defaultSettingsPage->AddTextInputItem(GitHub);
    defaultSettingsPage->AddTextInputItem(lic);
    defaultSettingsPage->AddTextInputItem(Author);
    defaultSettingsPage->AddTextInputItem(updateDate);
    defaultSettingsPage->AddTextInputItem(version);
    curSettingsPage = defaultSettingsPage;
    defaultSettingsPage->show();
    pageList.push_back(defaultSettingsPage);

    /************************/

    /* Initialize display area */
    QFont titleFont = QFont("Times", 24);
    QFontMetrics titleFm(titleFont);
    canvasTitle = new QLineEdit(this);
    canvasTitle->setFont(titleFont);
    canvasTitle->setText("START");
    canvasTitle->setMaxLength(20);
    canvasTitle->setReadOnly(true);
    canvasTitle->setMinimumHeight(titleFm.height());
    canvasTitle->setMaximumWidth(titleFm.size(Qt::TextSingleLine, "START").width() + 10);
    canvasTitle->setStyleSheet("background-color:#00000000;border-style:none;border-width:0px;margin-left:1px;");
    connect(canvasTitle, &QLineEdit::textEdited, canvasTitle, [=](QString text)
            { canvasTitle->setMaximumWidth(titleFm.size(Qt::TextSingleLine, text).width()); });

    QFont descFont = QFont("Times New Roman", 30);
    QFontMetrics descFm(descFont);
    canvasDesc = new QLineEdit(this);
    canvasDesc->setFont(descFont);
    canvasDesc->setText("Welcome and Greetings!");
    canvasDesc->setMaxLength(128);
    canvasDesc->setReadOnly(true);
    canvasDesc->setAlignment(Qt::AlignHCenter);
    canvasDesc->setMinimumHeight(descFm.lineSpacing());
    canvasDesc->setStyleSheet("background-color:#00000000;border-style:none;border-width:0px;");

    settingsIcon = new customIcon(":/icons/icons/settings_black.svg", "settings", 5, this);
    settingsIcon->setMinimumHeight(canvasTitle->height() * 0.7);
    settingsIcon->setMaximumWidth(canvasTitle->height() * 0.7);

    themeIcon = new customIcon(":/icons/icons/lightbulb_black.svg", "theme", 5, this);
    themeIcon->setMinimumHeight(canvasTitle->height() * 0.7);
    themeIcon->setMaximumWidth(canvasTitle->height() * 0.7);

    connect(settingsIcon, &customIcon::clicked, this, [=]()
            {
        QPropertyAnimation *rotate = new QPropertyAnimation(settingsIcon, "rotationAngle", this);
        rotate->setDuration(750);
        rotate->setStartValue(0);
        rotate->setEndValue(90);
        rotate->setEasingCurve(QEasingCurve::InOutExpo);
        rotate->start();
        curSettingsPage->slideIn(); });

    connect(themeIcon, &customIcon::clicked, this,
            [=](){
        theme_switch();
                });
    /* create title */

    QWidget *titleInnerWidget = new QWidget(this);
    titleInnerWidget->setFixedHeight(canvasTitle->height());
    QHBoxLayout *innerLayout = new QHBoxLayout(titleInnerWidget);
    titleInnerWidget->setLayout(innerLayout);
    innerLayout->setContentsMargins(0, 0, 0, 0);
    innerLayout->setSpacing(10);
    innerLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    innerLayout->addWidget(canvasTitle);
    innerLayout->addWidget(settingsIcon);
    innerLayout->addWidget(themeIcon);

    QWidget *titleWidget = new QWidget(this);
    titleWidget->setMaximumHeight(canvasTitle->height() + canvasDesc->height());
    QVBoxLayout *outerLayout = new QVBoxLayout(titleWidget);
    titleWidget->setLayout(outerLayout);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);
    outerLayout->addWidget(titleInnerWidget);
    outerLayout->addWidget(canvasDesc);

    /* create default page */
    defaultPage = new QWidget(ui->mainWidget);
    defaultPage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    Linked_List = new bigIconButton(":/icons/icons/linked_list.png", "Linked List", 10, this);
    Linked_List->setScale(0.7); // 调里面icon大小用的
    Queue = new bigIconButton(":/icons/icons/queue.png", "Queue", 10, this);
    Queue->setScale(0.3); // 调里面icon大小用的
    BST = new bigIconButton(":/icons/icons/tree.png", "Binary Tree", 10, this);
    BST->setScale(0.1); // 调里面icon大小用的

    QHBoxLayout *defaultPageLayout = new QHBoxLayout(defaultPage);
    defaultPage->setLayout(defaultPageLayout);
    defaultPageLayout->setSpacing(15);
    defaultPageLayout->addWidget(Linked_List);
    defaultPageLayout->addWidget(Queue);
    defaultPageLayout->addWidget(BST);

    connect(Linked_List, &bigIconButton::clicked,
            this, [=](){
                     MyListCanvas *newCanvas = new MyListCanvas(cornerRadius,
                                                                "Linked_List_" + QDateTime::currentDateTime().toString(),
                                                                "Linked List",
                                                                ui->mainWidget);
                     my_list_canvas = newCanvas;
                     pageList.push_back(newCanvas->settingPage());
                     selectListCanvas(newCanvas);
                     connect(newCanvas, &MyListCanvas::nameChanged, this, [=](QString text){
                         canvasTitle->setText(text);
                         canvasTitle->setMaximumWidth(QFontMetrics(QFont("Times", 24)).size(Qt::TextSingleLine, canvasTitle->text()).width() + 10);
                     });
                     connect(newCanvas, &MyListCanvas::descChanged, this, [=](QString text){this->canvasDesc->setText(text);});
                     connect(newCanvas, &MyListCanvas::setDel, this, [=](MyListCanvas *c){curSettingsPage->slideOut();deleteListCanvas(c);});
                     newCanvas->night_mode = night_mode;});

    ui->displayLayout->addWidget(titleWidget);
    ui->displayLayout->addWidget(defaultPage);
    ui->displayLayout->setAlignment(Qt::AlignTop);
}

void MainWindow::selectListCanvas(MyListCanvas *canvas)
{
    if (!curCanvas)
    {
        ui->displayLayout->removeWidget(defaultPage);
        defaultPage->hide();
        ui->displayLayout->addWidget(canvas);
        canvas->show();
    }
    else
    {
        ui->displayLayout->removeWidget(curCanvas);
        curCanvas->hide();
        ui->displayLayout->addWidget(canvas);
        canvas->show();
    }
    curCanvas = canvas;
    canvas->settingPage()->setParent(ui->mainWidget);
    curSettingsPage = canvas->settingPage();
    canvasTitle->setText(curCanvas->name());
    canvasTitle->setMaximumWidth(QFontMetrics(QFont("Times", 24)).size(Qt::TextSingleLine, canvasTitle->text()).width() + 10);
    canvasDesc->setText(curCanvas->description());
}

void MainWindow::deleteListCanvas(MyListCanvas *canvas)
{
    ui->displayLayout->removeWidget(curCanvas);
    curCanvas->hide();

    ui->displayLayout->addWidget(defaultPage);
    defaultPage->show();
    curCanvas = nullptr;
    canvasTitle->setText("START");
    canvasTitle->setMaximumWidth(QFontMetrics(QFont("Times", 24)).size(Qt::TextSingleLine, "START").width() + 10);
    canvasDesc->setText("Welcome and Greetings!");
    curSettingsPage = defaultSettingsPage;

    pageList.erase(pageList.begin() + pageList.indexOf(canvas->settingPage()));
    delete canvas;
    my_list_canvas = nullptr;
    ui->mainWidget->update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mousePressed = true;
        lastPos = event->globalPosition().toPoint() - this->frameGeometry().topLeft();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::NoButton)
        mousePressed = false;
    if (!mousePressed)
    {
        mouseState = 0;
        if (!maximized && abs(event->pos().x() - ui->mainWidget->pos().x()) < 5)
            mouseState |= AT_LEFT;
        if (!maximized && abs(event->pos().y() - ui->mainWidget->pos().y()) < 5)
            mouseState |= AT_TOP;
        if (!maximized && abs(event->pos().x() - ui->mainWidget->pos().x() - ui->mainWidget->width()) < 5)
            mouseState |= AT_RIGHT;
        if (!maximized && abs(event->pos().y() - ui->mainWidget->pos().y() - ui->mainWidget->height()) < 5)
            mouseState |= AT_BOTTOM;
        if (mouseState == AT_TOP_LEFT || mouseState == AT_BOTTOM_RIGHT)
            setCursor(Qt::SizeFDiagCursor);
        else if (mouseState == AT_TOP_RIGHT || mouseState == AT_BOTTOM_LEFT)
            setCursor(Qt::SizeBDiagCursor);
        else if (mouseState & (AT_LEFT | AT_RIGHT))
            setCursor(Qt::SizeHorCursor);
        else if (mouseState & (AT_TOP | AT_BOTTOM))
            setCursor(Qt::SizeVerCursor);
        else
            unsetCursor();
    }
    else
    {
        if (mouseState == 0)
        {
            if (maximized)
            {
                qreal wRatio = (double)event->pos().x() / (double)ui->mainWidget->width();
                controlWindowScale();
                this->move(QPoint(event->globalPosition().x() - ui->mainWidget->width() * wRatio, -30));
                lastPos = QPoint(ui->mainWidget->width() * wRatio, event->pos().y());
            }
            else
                this->move(event->globalPosition().toPoint() - lastPos);
        }
        else
        {
            QPoint d = event->globalPosition().toPoint() - frameGeometry().topLeft() - lastPos;
            if (mouseState & AT_LEFT)
            {
                this->move(this->frameGeometry().x() + d.x(), this->frameGeometry().y());
                this->resize(this->width() - d.x(), this->height());
            }
            if (mouseState & AT_RIGHT)
            {
                this->resize(this->width() + d.x(), this->height());
            }
            if (mouseState & AT_TOP)
            {
                this->move(this->frameGeometry().x(), this->frameGeometry().y() + d.y());
                this->resize(this->width(), this->height() - d.y());
            }
            if (mouseState & AT_BOTTOM)
            {
                this->resize(this->width(), this->height() + d.y());
            }
        }
        lastPos = event->globalPosition().toPoint() - this->frameGeometry().topLeft();
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    // Resize border
    if (border)
        border->resize(ui->mainWidget->size() + QSize(2, 2));

    // Resize mask
    QPainterPath path;
    path.addRoundedRect(ui->mainWidget->rect(), cornerRadius - 1, cornerRadius - 1);
    QRegion mask(path.toFillPolygon().toPolygon());
    ui->mainWidget->setMask(mask);

    // Resize all pages
    for (int i = 0; i < pageList.size(); i++)
    {
        pageList[i]->resize(ui->mainWidget->width() * 0.3 < pageList[i]->preferWidth ? pageList[i]->preferWidth - 1 : ui->mainWidget->width() * 0.3 - 1, ui->mainWidget->height());
        pageList[i]->resize(pageList[i]->width() + 1, pageList[i]->height());
    }
}

void MainWindow::controlWindowScale()
{
    if (!maximized)
    {
        lastGeometry = this->frameGeometry();
        windowShadow->setEnabled(false);
        ui->verticalLayout->setContentsMargins(0, 0, 0, 0);
        border->hide();
        QString mainStyle = "QWidget#mainWidget{background-color:" + mainBackGround.name() + ";border-radius:0px;}";
        ui->mainWidget->setStyleSheet(mainStyle);
        this->showMaximized();
        maximized = true;
        QPainterPath path;
        path.addRect(ui->mainWidget->rect());
        QRegion mask(path.toFillPolygon().toPolygon());
        ui->mainWidget->setMask(mask);
    }
    else
    {
        ui->verticalLayout->setContentsMargins(30, 30, 30, 30);
        this->showNormal();
        QString mainStyle = "QWidget#mainWidget{background-color:" + mainBackGround.name() + QString::asprintf(";border-radius:%dpx", cornerRadius) + "}";
        ui->mainWidget->setStyleSheet(mainStyle);
        QPainterPath path;
        path.addRoundedRect(ui->mainWidget->rect(), cornerRadius - 1, cornerRadius - 1);
        QRegion mask(path.toFillPolygon().toPolygon());
        ui->mainWidget->setMask(mask);
        border->show();
        windowShadow->setEnabled(true);
        this->resize(lastGeometry.width(), lastGeometry.height());
        this->move(lastGeometry.x(), lastGeometry.y());
        maximized = false;
    }
}

void MainWindow::theme_switch()
{
    switch (theme) {
    case Theme_write: theme = Theme_black; night_mode = true; break;
    case Theme_black: theme = Theme_write; night_mode = false; break;
    default: theme = Theme_black; break;
    }
    theme_refresh();
}

void MainWindow::theme_refresh()
{
    QColor background_color;
    QColor content_color;
    switch (theme) {
    case Theme_write:
        background_color = Qt::white;
        content_color = Qt::black;
        Linked_List->changeIcon(":/icons/icons/linked_list.png");
        Queue->changeIcon(":/icons/icons/queue.png");
        BST->changeIcon(":/icons/icons/tree.png");
        settingsIcon->changeIcon(":/icons/icons/settings_black.svg");
        themeIcon->changeIcon(":/icons/icons/lightbulb_black.svg");
        break;
    case Theme_black:
        background_color = Qt::black;
        content_color = Qt::white;
        night_mode = true;
        Linked_List->changeIcon(":/icons/icons/linked_list_white.png");
        Queue->changeIcon(":/icons/icons/queue_white.png");
        BST->changeIcon(":/icons/icons/tree_white.png");
        settingsIcon->changeIcon(":/icons/icons/settings_white.svg");
        themeIcon->changeIcon(":/icons/icons/lightbulb_white.svg");
        break;
    default:
        return;
    }
    setStyleSheet(QString("background-color:%1;color:%2;")
                  .arg(background_color.name(), content_color.name()));
    settingsIcon->setDefaultColor(background_color);
    settingsIcon->setHoverColor(background_color);
    themeIcon->setDefaultColor(background_color);
    themeIcon->setHoverColor(background_color);
    foreach (SlidePage *page, pageList) {
        page->setTheme(night_mode);
    }

    Linked_List->setScale(0.7); // 调里面icon大小用的
    Queue->setScale(0.3); // 调里面icon大小用的
    BST->setScale(0.1); // 调里面icon大小用的

    if(my_list_canvas)
        my_list_canvas->setTheme(night_mode);
}
