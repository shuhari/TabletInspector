#include "stdafx.h"
#include "MainWindow.h"
#include "resource.h"


MainWindow::MainWindow() 
    : _pReader(nullptr) {
}


BOOL MainWindow::PreTranslateMessage(MSG* pMsg) {
    if (CFrameWindowImpl<MainWindow>::PreTranslateMessage(pMsg))
        return TRUE;
    return FALSE;
}


BOOL MainWindow::OnIdle() {
    UIUpdateToolBar();
    UIUpdateStatusBar();
    return TRUE;
}


int MainWindow::onCreate(LPCREATESTRUCT pCreateStruct) {
    createChildren();
    onInitialUpdate();

    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != nullptr);
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);

    return 0;
}


void MainWindow::onDestroy() {
    if (_pReader) {
        _pReader->close();
        _pReader->waitForExit(5000);
        _pReader = nullptr;
    }
    PostQuitMessage(0);
}


void MainWindow::onFileExit(UINT, int, CWindow) {
    PostMessage(WM_CLOSE);
}


void MainWindow::onViewToolbar(UINT, int, CWindow) {
    CWindow toolbar = m_hWndToolBar;
    BOOL visible = !toolbar.IsWindowVisible();
    toolbar.ShowWindow(visible ? SW_SHOWNOACTIVATE : SW_HIDE);
    UISetCheck(ID_VIEW_TOOLBAR, visible);
    UpdateLayout();
}


void MainWindow::onViewStatusBar(UINT, int, CWindow) {
    CWindow statusBar = m_hWndStatusBar;
    BOOL visible = !statusBar.IsWindowVisible();
    statusBar.ShowWindow(visible ? SW_SHOWNOACTIVATE : SW_HIDE);
    UISetCheck(ID_VIEW_STATUS_BAR, visible);
    UpdateLayout();
}


void MainWindow::onViewLogs(UINT, int, CWindow) {
    int singleMode = _mainSplitter.GetSinglePaneMode();
    showLogs(singleMode == SPLIT_PANE_TOP);
}


void MainWindow::onHelpAbout(UINT, int, CWindow) {
}


void MainWindow::onPaneClose(UINT, int, CWindow wndFocus) {
    if (wndFocus == _logList.GetParent()) {
        showLogs(false);
    }
}


void MainWindow::createChildren() {
    CreateSimpleToolBar();
    CreateSimpleStatusBar();

    _mainSplitter.Create(*this, rcDefault);
    _logList.CreateInPane(_mainSplitter);

    _clientSplitter.Create(_mainSplitter, rcDefault);
    _canvasView.Create(_clientSplitter, rcDefault);
    _detailView.Create(_clientSplitter, rcDefault,
        NULL,
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        WS_EX_CLIENTEDGE);

    m_hWndClient = _mainSplitter;
    UpdateLayout();
    _mainSplitter.SetSplitterPanes(_clientSplitter, _logList.GetParent());
    _mainSplitter.SetSplitterPos(600);
    _clientSplitter.SetSplitterPanes(_detailView, _canvasView);
    _clientSplitter.SetSplitterPos(600);
}


void MainWindow::onInitialUpdate() {

    UISetCheck(ID_VIEW_TOOLBAR, true);
    UISetCheck(ID_VIEW_STATUS_BAR, true);
    UISetCheck(ID_VIEW_LOGS, true);

    _logList.onInitialUpdate();
    _detailView.onInitialUpdate();

    detectTablets();
}


void MainWindow::showLogs(bool show) {
    if (show) {
        _mainSplitter.SetSinglePaneMode(SPLIT_PANE_NONE);
    }
    else {
        _mainSplitter.SetSinglePaneMode(SPLIT_PANE_TOP);
    }
    UISetCheck(ID_VIEW_LOGS, show);
}


void MainWindow::onTabletConnected(PCWSTR szDevicePath) {
    _logList.info(IDS_LOG_TABLET_CONNECTED, szDevicePath);
    if (_pReader) {
        _pReader->close();
        _pReader = nullptr;
    }
    _pReader = new TabletReader(m_hWnd);
    if (_pReader->open(szDevicePath)) {
        _detailView.setTabletInfo(&_pReader->tabletInfo());
    }
    else {
        _pReader->close();
        delete _pReader;
        _pReader = nullptr;
    }
}


void MainWindow::onTabletDisconnected(PCWSTR szDevicePath) {
    _logList.info(IDS_LOG_TABLET_DISCONNECTED);
    _detailView.setTabletInfo(nullptr);
}

