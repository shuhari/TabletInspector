#include "stdafx.h"
#include "MainWindow.h"
#include "Resources.h"
#include "Public.h"
#include "SettingsDialog.h"


MainWindow::MainWindow(QWidget *parent) : 
    QMainWindow(parent),  
    _usbReader(nullptr) {
    

    setWindowTitle(tr("Tablet Inspector"));
    setWindowIcon(ImageRegistry::icon(ImageKey::app));
    setMinimumSize(800, 600);

    createActions();
    createDocks();
    createCentral();
    createToolBar();
    createStatusBar();
    createMenuBar();

    _usbDetector = new UsbDetector(this, GUID_DEVINTERFACE_TABLET_WINUSB);
    connect(_usbDetector, &UsbDetector::deviceConnected, this, &MainWindow::onDeviceConnected);
    connect(_usbDetector, &UsbDetector::deviceDisconnected, this, &MainWindow::onDeviceDisconnected);

    qApp->installNativeEventFilter(this);
    _usbDetector->detectOnStartUp();
}


void MainWindow::createActions() {
    _actions[fileExit] = new QAction(tr("E&xit"));
    _actions[viewStatusBar] = new QAction(tr("&Status Bar"));
    _actions[viewStatusBar]->setCheckable(true);
    _actions[toolSettings] = new QAction(tr("&Settings..."));
    _actions[toolSettings]->setIcon(ImageRegistry::icon(ImageKey::settings));
    _actions[helpAbout] = new QAction(tr("&About"));

    connect(_actions[fileExit], &QAction::triggered, this, &MainWindow::onFileExit);
    connect(_actions[viewStatusBar], &QAction::triggered, this, &MainWindow::onViewStatusBar);
    connect(_actions[toolSettings], &QAction::triggered, this, &MainWindow::onToolSettings);
    connect(_actions[helpAbout], &QAction::triggered, this, &MainWindow::onHelpAbout);
}


void MainWindow::createCentral() {
    _canvas = new CanvasWidget();
    setCentralWidget(_canvas);
}


void MainWindow::createDocks() {
    _logList = new LogList();
    _tabletInfoPage = new TabletInfoPage();

    auto propTab = new QTabWidget();
    propTab->addTab(_tabletInfoPage, tr("Tablet Information"));

    auto leftDock = newDock(tr("Information"), propTab, Qt::LeftDockWidgetArea);
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
    toolbar->addSeparator();
    toolbar->addAction(_actions[toolSettings]);

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

    auto toolMenu = new QMenu(tr("&Tools"));
    toolMenu->addAction(_actions[toolSettings]);

    auto helpMenu = new QMenu(tr("&Help"));
    helpMenu->addAction(_actions[helpAbout]);

    bar->addMenu(fileMenu);
    bar->addMenu(viewMenu);
    bar->addMenu(toolMenu);
    bar->addMenu(helpMenu);
}


/*bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result) {

    MSG *msg = (MSG*)message;
    return false;
}*/

void MainWindow::closeEvent(QCloseEvent* event) {
    closeReader();
    event->accept();
}

bool MainWindow::nativeEventFilter(const QByteArray &eventType, void *message, long *result) {
    
    MSG *msg = (MSG*)message;
    if (msg->message == WM_DEVICECHANGE && _usbDetector != nullptr) {
        _usbDetector->detectOnDeviceChangeEvent(*msg);
    }
    return false;
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


void MainWindow::onToolSettings() {
    SettingsDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
    }
}


void MainWindow::onHelpAbout() {

}


void MainWindow::onDeviceConnected(const QString& devicePath) {
    closeReader();

    _logList->info(tr("Tablet connected: %1").arg(devicePath));

    auto reader = new UsbReader();
    if (reader->open(devicePath)) {
        _usbReader = reader;
        auto& tabletInfo = reader->tabletInfo();
        _connectionIndicator->setConnected(tabletInfo.model);
        _tabletInfoPage->setInfo(&tabletInfo);
    }
}


void MainWindow::onDeviceDisconnected(const QString& devicePath) {
    closeReader();

    _logList->info(tr("Tablet disconnected: %1").arg(devicePath));
    _connectionIndicator->setDisconnected();
    _tabletInfoPage->setInfo(nullptr);
}


void MainWindow::closeReader() {
    if (_usbReader != nullptr) {
        _usbReader->close();
        delete _usbReader;
        _usbReader = nullptr;
    }
}
