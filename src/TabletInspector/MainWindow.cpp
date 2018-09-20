#include "stdafx.h"
#include "MainWindow.h"
#include "Resources.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)  {
    
    setWindowTitle(tr("Tablet Inspector"));
    setWindowIcon(ImageRegistry::icon(ImageKey::app));
    setMinimumSize(800, 600);

    createActions();
    createDocks();
    createCentral();
    createToolBar();
    createStatusBar();
    createMenuBar();
}


void MainWindow::createActions() {
    _actions[fileExit] = new QAction(tr("E&xit"));
    connect(_actions[fileExit], &QAction::triggered, this, &MainWindow::onFileExit);

    _actions[viewStatusBar] = new QAction(tr("&Status Bar"));
    _actions[viewStatusBar]->setCheckable(true);
    connect(_actions[viewStatusBar], &QAction::triggered, this, &MainWindow::onViewStatusBar);

    _actions[helpAbout] = new QAction(tr("&About"));
    connect(_actions[helpAbout], &QAction::triggered, this, &MainWindow::onHelpAbout);
}


void MainWindow::createCentral() {
    _canvas = new CanvasWidget();
    setCentralWidget(_canvas);
}


void MainWindow::createDocks() {
    _logList = new LogList();

    auto leftDock = newDock(tr("Information"), nullptr, Qt::LeftDockWidgetArea);
    auto rightDock = newDock(tr("Data"), nullptr, Qt::RightDockWidgetArea);
    auto bottomDock = newDock(tr("Logs"), _logList, Qt::BottomDockWidgetArea);

    _actions[viewProp] = leftDock->toggleViewAction();
    _actions[viewProp]->setIcon(ImageRegistry::icon(ImageKey::prop));

    _actions[viewData] = rightDock->toggleViewAction();
    _actions[viewData]->setIcon(ImageRegistry::icon(ImageKey::data));

    _actions[viewLogs] = bottomDock->toggleViewAction();
    _actions[viewLogs]->setIcon(ImageRegistry::icon(ImageKey::logs));
}


QDockWidget* MainWindow::newDock(const QString& title, QWidget* widget, Qt::DockWidgetArea area) {
    auto dock = new QDockWidget(title, this);
    dock->setFeatures(QDockWidget::DockWidgetClosable);
    dock->setAllowedAreas(area);
    if (widget)
        dock->setWidget(widget);
    addDockWidget(area, dock);
    return dock;
}


void MainWindow::createToolBar() {
    auto toolbar = addToolBar(tr("Toolbar"));
    
    toolbar->addAction(_actions[viewProp]);
    toolbar->addAction(_actions[viewData]);
    toolbar->addAction(_actions[viewLogs]);

    _actions[viewToolBar] = toolbar->toggleViewAction();
}


void MainWindow::createStatusBar() {
    auto bar = statusBar();
    
    _connectionIndicator = new ConnectionIndicator();
    bar->addPermanentWidget(_connectionIndicator);
    _connectionIndicator->setDisconnected();
}


void MainWindow::createMenuBar() {
    _actions[viewStatusBar]->setChecked(true);

    auto bar = menuBar();

    auto fileMenu = new QMenu(tr("&File"));
    fileMenu->addAction(_actions[fileExit]);

    auto viewMenu = new QMenu(tr("&View"));
    viewMenu->addAction(_actions[viewToolBar]);
    viewMenu->addAction(_actions[viewStatusBar]);
    viewMenu->addSeparator();
    viewMenu->addAction(_actions[viewProp]);
    viewMenu->addAction(_actions[viewData]);
    viewMenu->addAction(_actions[viewLogs]);

    auto helpMenu = new QMenu(tr("&Help"));
    helpMenu->addAction(_actions[helpAbout]);

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
    _actions[viewStatusBar]->setChecked(visible);
}


void MainWindow::onHelpAbout() {

}