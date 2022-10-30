#include "./head/mylistcanvas.h"
#include <QFileDialog>
MyListCanvas::MyListCanvas(int radius, QString name, QString desc, QWidget *parent)
    : QWidget(parent)
    , canvasName(name)
    , canvasDescription(desc)
{
    /* create canvas */
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainLayout);

    view = new MyListView();
    view->setSceneRect(view->rect());
    view->setStyleSheet("background-color: #FFFFFF;border:1px solid #cfcfcf;border-radius:10px;");
    mainLayout->addWidget(view);
    this->setFocusPolicy(Qt::ClickFocus);
    CreateSettings(radius);
}

void MyListCanvas::CreateSettings(int radius)
{
    /* create settings page */
    settings = new SlidePage(radius, "SETTINGS", this->parentWidget());
    QWidget *whiteSpace = new QWidget(this);
    whiteSpace->setFixedHeight(30);
    textInputItem *rename = new textInputItem("Name", this);
    rename->setValue(canvasName);
    connect(rename, &textInputItem::textEdited, this, [=](QString text)
            {canvasName = text; emit nameChanged(text); });
    textInputItem *redesc = new textInputItem("Detail", this);
    redesc->setValue(canvasDescription);
    connect(redesc, &textInputItem::textEdited, this, [=](QString text)
            {canvasDescription = text; emit descChanged(text); });
    QWidget *whiteSpace2 = new QWidget(this);
    whiteSpace2->setFixedHeight(30);
    textButton *saveBtn = new textButton("Save to file", this);
    connect(saveBtn, &textButton::clicked, this, [=]()
            {
                });
    textButton *delBtn = new textButton("Delete", this);
    connect(delBtn, &textButton::clicked, this, [=]()
            { emit setDel(this); });
    settings->AddTextButton(delBtn);
    settings->AddTextButton(saveBtn);
    settings->AddWhiteSpace(whiteSpace2);
    settings->AddWhiteSpace(whiteSpace);
    settings->AddTextInputItem(redesc);
    settings->AddTextInputItem(rename);
    settings->show();

    QTimer *delay = new QTimer(this);
    connect(delay, &QTimer::timeout, this, [=]()
            { Init(); setTheme(night_mode);});
    delay->setSingleShot(true);
    delay->start(10);
}

void MyListCanvas::Init()
{
    /* Create info widget */
    infoWidget = new QWidget(this);
    mainLayout->addWidget(infoWidget);
    mainLayout->setStretch(0, 7);
    mainLayout->setStretch(1, 3);
    infoWidget->setMinimumWidth(250);
    infoWidget->setMaximumWidth(500);

    // Set basic layout
    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
    infoWidget->setLayout(infoLayout);
    infoLayout->setContentsMargins(10, 0, 0, 0);
    infoLayout->setAlignment(Qt::AlignTop);

    QFont titleFont = QFont("Corbel", 20);

    QWidget *upper = new QWidget(infoWidget);
    QVBoxLayout *upperLayout = new QVBoxLayout(upper);
    upper->setLayout(upperLayout);
    upperLayout->setContentsMargins(0, 0, 0, 0);
    upper->setContentsMargins(0, 0, 0, 0);
    pageName = new QLabel(infoWidget);
    pageName->setText("INFO");
    pageName->setFont(titleFont);
    pageName->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    QWidget *upperSplitter = new QWidget(upper);
    upperSplitter->setFixedSize(30, 6);
    upperSplitter->setStyleSheet("background-color:#3c3c3c;border-radius:3px;");
    upperLayout->addWidget(pageName);
    upperLayout->addWidget(upperSplitter);

    QWidget *lower = new QWidget(infoWidget);
    QVBoxLayout *lowerLayout = new QVBoxLayout(lower);
    lower->setLayout(lowerLayout);
    lowerLayout->setContentsMargins(0, 0, 0, 0);
    logLabel = new QLabel(lower);
    logLabel->setText("LOG");
    logLabel->setFont(titleFont);
    logLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    QWidget *lowerSplitter = new QWidget(lower);
    lowerSplitter->setFixedSize(30, 6);
    lowerSplitter->setStyleSheet("background-color:#3c3c3c;border-radius:3px;");
    ScrollAreaCustom *logDisplay = new ScrollAreaCustom(lower);
    logDisplay->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    lowerLayout->addWidget(logLabel);
    lowerLayout->addWidget(lowerSplitter);
    lowerLayout->addWidget(logDisplay);

    infoLayout->addWidget(upper);
    infoLayout->addWidget(lower);

    // Add specific items and connections
    // Default page
    QWidget *defInfoPage = new QWidget(infoWidget);
    QVBoxLayout *defInfoLayout = new QVBoxLayout(defInfoPage);
    defInfoPage->setLayout(defInfoLayout);
    defInfoLayout->setContentsMargins(0, 0, 0, 0);
    defInfoLayout->setAlignment(Qt::AlignTop);
    QWidget *defTextItems = new QWidget(defInfoPage);
    defTextItems->setObjectName("DefTextItems");
    defTextItems->setStyleSheet("QWidget#DefTextItems{border:1px solid #cfcfcf;border-radius:5px;}");
    QVBoxLayout *defTextLayout = new QVBoxLayout(defTextItems);
    defTextItems->setLayout(defTextLayout);
    defTextLayout->setContentsMargins(0, 5, 0, 5);
    textInputItem *textName = new textInputItem("Name", defInfoPage);
    text_input_items.append(textName);
    textName->setValue(canvasName);
    connect(this, &MyListCanvas::nameChanged, this, [=]()
            { textName->setValue(canvasName); });
    textName->setEnabled(false);
    defTextLayout->addWidget(textName);
    textInputItem *textDesc = new textInputItem("Detail", defInfoPage);
    text_input_items.append(textDesc);
    textDesc->setValue(canvasDescription);
    connect(this, &MyListCanvas::descChanged, this, [=]()
            { textDesc->setValue(canvasDescription); });
    textDesc->setEnabled(false);
    defTextLayout->addWidget(textDesc);
    textInputItem *vexNumText = new textInputItem("Vex", defInfoPage);
    text_input_items.append(vexNumText);
    vexNumText->setValue(QString::asprintf("%d", view->vexNum));
    vexNumText->setEnabled(false);
    defTextLayout->addWidget(vexNumText);
    textInputItem *arcNumText = new textInputItem("Arc", defInfoPage);
    text_input_items.append(arcNumText);
    arcNumText->setValue(QString::asprintf("%d", view->arcNum));
    arcNumText->setEnabled(false);
    defTextLayout->addWidget(arcNumText);
    defInfoLayout->addWidget(defTextItems);
    upperLayout->addWidget(defInfoPage);
    defInfoPage->show();

    // VexPage
    QWidget *vexInfoPage = new QWidget(infoWidget);
    QVBoxLayout *vexInfoLayout = new QVBoxLayout(vexInfoPage);
    vexInfoLayout->setContentsMargins(0, 0, 0, 0);
    vexInfoLayout->setAlignment(Qt::AlignTop);
    vexInfoPage->setLayout(vexInfoLayout);
    QWidget *vexTextItems = new QWidget(vexInfoPage);
    vexTextItems->setObjectName("VexTextItems");
    vexTextItems->setStyleSheet("QWidget#VexTextItems{border:1px solid #cfcfcf;border-radius:5px;}");
    QVBoxLayout *vexTextLayout = new QVBoxLayout(vexTextItems);
    vexTextItems->setLayout(vexTextLayout);
    vexTextLayout->setContentsMargins(0, 5, 0, 5);
    textInputItem *textTag = new textInputItem("Tag", vexInfoPage);
    vexTextLayout->addWidget(textTag);
    textInputItem *dijStart = new textInputItem("Start", vexInfoPage);
    dijStart->setValue("Run dijkstra first");
    dijStart->setEnabled(false);
    vexTextLayout->addWidget(dijStart);
    textInputItem *dijDistance = new textInputItem("Dist", vexInfoPage);
    dijDistance->setValue("Infinite");
    dijDistance->setEnabled(false);
    vexTextLayout->addWidget(dijDistance);
    textInputItem *dijPrev = new textInputItem("Prev", vexInfoPage);
    dijPrev->setValue("Run dijkstra first");
    dijPrev->setEnabled(false);
    vexTextLayout->addWidget(dijPrev);
    vexInfoLayout->addWidget(vexTextItems);
    QWidget *traverseBar = new QWidget(vexInfoPage);
    QHBoxLayout *traverseLayout = new QHBoxLayout(traverseBar);
    traverseBar->setLayout(traverseLayout);
    traverseLayout->setContentsMargins(0, 0, 0, 0);
    textButton *startBfs = new textButton("BFS", vexInfoPage);
    traverseLayout->addWidget(startBfs);
    textButton *startDfs = new textButton("DFS", vexInfoPage);
    traverseLayout->addWidget(startDfs);
    vexInfoLayout->addWidget(traverseBar);
    textButton *startDij = new textButton("Start Dijkstra", vexInfoPage);
    startDij->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    vexInfoLayout->addWidget(startDij);
    textButton *delVex = new textButton("Delete", "#1acb1b45", "#2acb1b45", "#3acb1b45", vexInfoPage);
    vexInfoLayout->addWidget(delVex);
    upperLayout->addWidget(vexInfoPage);
    vexInfoPage->hide();

    connect(view, &MyListView::info_changed,
            this, [=](){
                        vexNumText->setValue(QString::asprintf("%d", view->vexNum));
                        arcNumText->setValue(QString::asprintf("%d", view->arcNum));});
    connect(view, &MyListView::selected,
            this, [=](QGraphicsItem *item){
                        defInfoPage->hide();
                        vexInfoPage->show();});
    connect(view, &MyListView::log_signal,
            this, [=](QString log_info){
                        logDisplay->addWidget(new viewLog(log_info));});
    connect(view, &MyListView::unselected,
            this, [=](){
                        vexInfoPage->hide();
                        defInfoPage->show();});
}

void MyListCanvas::setTheme(bool night_mode)
{
    settings->setTheme(night_mode);
    view->setTheme(night_mode);
    foreach (textInputItem *it, text_input_items) {
        it->setTheme(night_mode);
    }
    foreach (textButton *it, text_buttons) {
        it->setTheme(night_mode);
    }
}
