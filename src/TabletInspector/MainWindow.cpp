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
    createToolBar();
    createStatusBar();
    createSideBars();
    createCentral();
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
    stopReader();
    event->accept();
}


void MainWindow::onInitialUpdate() {
    _actions[viewStatusBar]->setChecked(true);

    _tabletDetector->detectOnStartup();
}


void MainWindow::stopReader() {
    if (_tabletReader) {
        // Reader delete itself when thread terminated
        _tabletReader->stopRead();
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

}


void MainWindow::createCentral() {

}

void MainWindow::createMenuBar() {
    auto mainMenu = menuBar();

    SubMenu(mainMenu, Strings::menuFile())
        .action(_actions[fileExit]);

    SubMenu(mainMenu, Strings::menuView())
        .action(_actions[viewToolBar])
        .action(_actions[viewStatusBar])
        .separator();

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
        _tabletReader = reader;
        auto& tabletInfo = reader->info();
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
    stopReader();
}


void MainWindow::onTabletReadData(const QByteArray& buffer) {

}


void MainWindow::onTabletReadError(DWORD dwError) {

}