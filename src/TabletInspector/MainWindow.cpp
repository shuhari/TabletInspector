#include "stdafx.h"
#include "MainWindow.h"
#include "Strings.h"
#include "Resources.h"
#include "Public.h"
#include "QtHelper.h"
#include "SettingsDialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _tabletDetector(nullptr),
    _tabletReader(nullptr) {

    setWindowTitle(Strings::appTitle());
    setWindowIcon(Resources::appIcon());
    setMinimumSize(800, 600);

    createActions();
    createSideBars();
    createCentral();
    createToolBar();
    createStatusBar();
    createMenuBar();

    _tabletDetector = new TabletDetector(this, GUID_DEVINTERFACE_TABLET_WINUSB);
    connect(_tabletDetector, &TabletDetector::tabletConnected, this, &MainWindow::onTabletConnected);
    connect(_tabletDetector, &TabletDetector::tabletDisconnected, this, &MainWindow::onTabletDisconnected);

    onInitialUpdate();
}


bool MainWindow::nativeEvent(const QByteArray& eventType, void* message, long* result) {
    MSG* msg = (MSG*)message;
    if (msg->message == WM_DEVICECHANGE && _tabletDetector) {
        _tabletDetector->detectOnDeviceChange(*msg);
    }
    return false;
}


void MainWindow::closeEvent(QCloseEvent* event) {
    stopReader(true);
    event->accept();
}


void MainWindow::onInitialUpdate() {
    _actions[viewStatusBar]->setChecked(true);

    _tabletDetector->detectOnStartup();
}


void MainWindow::stopReader(bool wait) {
    if (_tabletReader) {
        // Reader delete itself when thread terminated
        _tabletReader->stopRead();
        if (wait)
            _tabletReader->waitQuit(5000);
        _tabletReader = nullptr;
    }
}


QAction* MainWindow::createAction(Actions key, const QString text, void (MainWindow::* slot)(),
    QIcon icon/*=QIcon()*/, bool checkable/*=false*/) {
    QAction* action = new QAction(text);
    _actions[key] = action;
    connect(action, &QAction::triggered, this, slot);

    if (!icon.isNull())
        action->setIcon(icon);
    action->setCheckable(checkable);

    return action;
}


void MainWindow::createActions() {
    createAction(fileExit, Strings::actionFileExit(), &MainWindow::onFileExit);

    createAction(viewStatusBar, Strings::actionViewStatusBar(), &MainWindow::onViewStatusBar,
        QIcon(), true);

    createAction(toolSettings, Strings::actionToolSettings(), &MainWindow::onToolSettings,
        Resources::settings());

    createAction(helpAbout, Strings::actionHelpAbout(), &MainWindow::onHelpAbout);
}


void MainWindow::createToolBar() {
    auto toolbar = addToolBar(Strings::actionViewToolBar());
    
    toolbar->addAction(_actions[viewLogs]);
    toolbar->addAction(_actions[viewProp]);
    toolbar->addAction(_actions[viewData]);
    toolbar->addSeparator();
    toolbar->addAction(_actions[toolSettings]);

    _actions[viewToolBar] = toolbar->toggleViewAction();
}


void MainWindow::createStatusBar() {
    auto status = statusBar();

    _connectionIndicator = new ConnectionIndicator(status);
    status->addPermanentWidget(_connectionIndicator);
    _connectionIndicator->setDisconnected();
}


void MainWindow::createSideBars() {
    _logList = new LogList();
    _infoPage = new TabletInfoPage();
    _realTimePage = new RealTimePage();
    _hexPage = new HexPage();

    auto leftTab = new QTabWidget();
    leftTab->setTabPosition(QTabWidget::South);
    leftTab->addTab(_infoPage, Strings::tabletInfo());
    leftTab->addTab(_realTimePage, Strings::realTime());

    auto rightTab = new QTabWidget();
    rightTab->setTabPosition(QTabWidget::South);
    rightTab->addTab(_hexPage, Strings::hex());

    auto bottomDock = createDock(Strings::logs(), _logList,
        Qt::BottomDockWidgetArea, Qt::BottomDockWidgetArea,
        viewLogs, Resources::logs());
    auto leftDock = createDock(Strings::properties(), leftTab,
        Qt::LeftDockWidgetArea, Qt::LeftDockWidgetArea,
        viewProp, Resources::prop());
    leftDock->setMinimumWidth(300);
    auto rightDock = createDock(Strings::data(), rightTab,
        Qt::RightDockWidgetArea, Qt::RightDockWidgetArea,
        viewData, Resources::data());
    rightDock->setMinimumWidth(300);
}


QDockWidget* MainWindow::createDock(const QString& title,
    QWidget* widget,
    Qt::DockWidgetAreas allowAreas, Qt::DockWidgetArea initialArea,
    Actions actionKey, QIcon actionIcon) {

    auto dock = new QDockWidget(title, this);
    dock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    dock->setAllowedAreas(allowAreas);
    if (widget)
        dock->setWidget(widget);
    addDockWidget(initialArea, dock);

    if (actionKey != Actions::None) {
        auto toggleAction = dock->toggleViewAction();
        if (!actionIcon.isNull())
            toggleAction->setIcon(actionIcon);
        _actions[actionKey] = toggleAction;
    }
    return dock;
}


void MainWindow::createCentral() {
    _canvas = new Canvas();
    setCentralWidget(_canvas);
}


void MainWindow::createMenuBar() {
    auto mainMenu = menuBar();

    SubMenu(mainMenu, Strings::menuFile())
        .action(_actions[fileExit]);

    SubMenu(mainMenu, Strings::menuView())
        .action(_actions[viewToolBar])
        .action(_actions[viewStatusBar])
        .separator()
        .action(_actions[viewLogs])
        .action(_actions[viewProp])
        .action(_actions[viewData]);

    SubMenu(mainMenu, Strings::menuTool())
        .action(_actions[toolSettings]);

    SubMenu(mainMenu, Strings::menuHelp())
        .action(_actions[helpAbout]);
}


void MainWindow::onFileExit() {
    close();
}


void MainWindow::onViewStatusBar() {
    // show/Hide status bar
    auto status = statusBar();
    bool visible = !status->isVisible();
    status->setVisible(visible);
    _actions[viewStatusBar]->setChecked(visible);
}


void MainWindow::onToolSettings() {
    SettingsDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
    }
}


void MainWindow::onHelpAbout() {

}


void MainWindow::onTabletConnected(const QString& devicePath) {
    stopReader();

    auto reader = new TabletReader(this);
    if (reader->open(devicePath)) {
        auto& tabletInfo = reader->info();
        _logList->info(Strings::msg_tabletConnected().arg(devicePath));
        _infoPage->setInfo(&tabletInfo);
        _realTimePage->setInfo(&tabletInfo);

        _tabletReader = reader;
        _connectionIndicator->setConnected(tabletInfo.tabletName());
        connect(reader, &TabletReader::tabletReadData, this, &MainWindow::onTabletReadData);
        connect(reader, &TabletReader::tabletReadError, this, &MainWindow::onTabletReadError);
        _tabletReader->startRead();
    } else {
        delete reader;
    }
}


void MainWindow::onTabletDisconnected(const QString& devicePath) {
    _connectionIndicator->setDisconnected();
    _logList->warn(Strings::msg_tabletDisconnected().arg(devicePath));
    _infoPage->setInfo(nullptr);
    _realTimePage->setInfo(nullptr);
    stopReader();
}


void MainWindow::onTabletReadData(const QByteArray& buffer) {
    _hexPage->addData(buffer);
    _realTimePage->setData(buffer);
}


void MainWindow::onTabletReadError(DWORD dwError) {
    QString msg = Strings::msg_tabletReadError().arg(dwError);
    _logList->error(msg);
}