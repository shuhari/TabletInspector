#include "stdafx.h"
#include "MainWindow.h"
#include "resource.h"


MainWindow::MainWindow() {
}


int MainWindow::onCreate(LPCREATESTRUCT pCreateStruct) {
    createChildren();
    onInitialUpdate();
    return 0;
}


void MainWindow::onDestroy() {
    PostQuitMessage(0);
}


void MainWindow::onFileExit(UINT, int, CWindow) {
    PostMessage(WM_CLOSE);
}


void MainWindow::onHelpAbout(UINT, int, CWindow) {
}


void MainWindow::createChildren() {
    _mainSplitter.Create(*this, rcDefault);

    _logListContainer.Create(_mainSplitter, IDS_LOGLIST_TITLE);
    _logList.Create(_logListContainer, rcDefault, NULL, 0, WS_EX_CLIENTEDGE);
    _logListContainer.SetClient(_logList);

    m_hWndClient = _mainSplitter;
    UpdateLayout();
    _mainSplitter.SetSplitterPanes(NULL, _logListContainer);
    _mainSplitter.SetSplitterPos(600);
}


void MainWindow::onInitialUpdate() {
    _logList.onInitialUpdate();

    detectTablets();
}

void MainWindow::onTabletConnected(PCWSTR szDevicePath) {
    _logList.info(IDS_LOG_TABLET_CONNECTED, szDevicePath);
}


void MainWindow::onTabletDisconnected(PCWSTR szDevicePath) {
    _logList.info(IDS_LOG_TABLET_DISCONNECTED);
}

