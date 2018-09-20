#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    setWindowTitle(tr("Tablet Inspector"));
    setWindowIcon(QIcon(":/images/app.ico"));
    setMinimumSize(800, 600);

    createActions();
    createDocks();
    createCentral();
    createToolBar();
    createStatusBar();
    createMenuBar();
}


MainWindow::~MainWindow() {

}


void MainWindow::createActions() {
    _acFileExit = new QAction(tr("E&xit"));
    connect(_acFileExit, &QAction::triggered, this, &MainWindow::onFileExit);

    _acViewStatusBar = new QAction(tr("&Status Bar"));
    _acViewStatusBar->setCheckable(true);
    connect(_acViewStatusBar, &QAction::triggered, this, &MainWindow::onViewStatusBar);

    _acHelpAbout = new QAction(tr("&About"));
    connect(_acHelpAbout, &QAction::triggered, this, &MainWindow::onHelpAbout);
}


void MainWindow::createDocks() {
    _logList = new LogListWidget();

    _propWidget = new PropertyWidget();

    QDockWidget* leftDock = createDock(tr("Properties"), _propWidget, Qt::LeftDockWidgetArea);
    QDockWidget* rightDock = createDock(tr("Data"), nullptr, Qt::RightDockWidgetArea);
    QDockWidget* bottomDock = createDock(tr("Logs"), _logList, Qt::BottomDockWidgetArea);

    _acViewProp = leftDock->toggleViewAction();
    _acViewProp->setIcon(QIcon(":/images/props.png"));
    _acViewData = rightDock->toggleViewAction();
    _acViewData->setIcon(QIcon(":/images/data.png"));
    _acViewLogs = bottomDock->toggleViewAction();
    _acViewLogs->setIcon(QIcon(":/images/logs.png"));
}


QDockWidget* MainWindow::createDock(const QString& title, QWidget* widget,
                                    Qt::DockWidgetArea area) {
    auto dock = new QDockWidget(title, this);
    dock->setFeatures(QDockWidget::DockWidgetClosable);
    dock->setAllowedAreas(area);
    if (widget)
        dock->setWidget(widget);
    addDockWidget(area, dock);
    return dock;
}


void MainWindow::createCentral() {
    _canvas = new CanvasWidget();
    setCentralWidget(_canvas);
}


void MainWindow::createToolBar() {
    auto toolbar = addToolBar(tr("Toolbar"));
    toolbar->addAction(_acViewProp);
    toolbar->addAction(_acViewData);
    toolbar->addAction(_acViewLogs);

    _acViewToolBar = toolbar->toggleViewAction();
}


void MainWindow::createStatusBar() {
    auto status = statusBar();

    _indicator = new TabletIndicatorWidget();
    status->addPermanentWidget(_indicator);
    _indicator->setDisconnected();
}


void MainWindow::createMenuBar() {
    _acViewStatusBar->setChecked(true);

    auto bar = menuBar();

    auto fileMenu = new QMenu(tr("&File"));
    fileMenu->addAction(_acFileExit);

    auto viewMenu  =new QMenu(tr("&View"));
    viewMenu->addAction(_acViewToolBar);
    viewMenu->addAction(_acViewStatusBar);
    viewMenu->addSeparator();
    viewMenu->addAction(_acViewProp);
    viewMenu->addAction(_acViewData);
    viewMenu->addAction(_acViewLogs);

    auto helpMenu = new QMenu(tr("&Help"));
    helpMenu->addAction(_acHelpAbout);

    bar->addMenu(fileMenu);
    bar->addMenu(viewMenu);
    bar->addMenu(helpMenu);
}



void MainWindow::onFileExit() {
    close();
}



void MainWindow::onViewStatusBar() {
    auto bar = statusBar();
    bool visible = !bar->isVisible();
    bar->setVisible(visible);
    _acViewStatusBar->setChecked(visible);
}


void MainWindow::onHelpAbout() {

}
