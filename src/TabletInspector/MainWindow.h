#pragma once


#include "resource.h"
#include "LogListView.h"
#include "CanvasView.h"
#include "DetailView.h"
#include "TabletDetecter.h"
#include "TabletReader.h"


class MainWindow : public CFrameWindowImpl<MainWindow>,
    public CUpdateUI<MainWindow>,
    public CMessageFilter,
    public CIdleHandler,
    public TabletDetecter<MainWindow> {
public:
    DECLARE_FRAME_WND_CLASS(L"TabletInspector_MainWindow", IDR_MAINFRAME)

    BEGIN_MSG_MAP_EX(MainWindow)
        MSG_WM_CREATE(onCreate)
        MSG_WM_DESTROY(onDestroy)

        COMMAND_ID_HANDLER_EX(ID_FILE_EXIT, onFileExit)
        COMMAND_ID_HANDLER_EX(ID_VIEW_TOOLBAR, onViewToolbar)
        COMMAND_ID_HANDLER_EX(ID_VIEW_STATUS_BAR, onViewStatusBar)
        COMMAND_ID_HANDLER_EX(ID_VIEW_LOGS, onViewLogs)
        COMMAND_ID_HANDLER_EX(ID_HELP_ABOUT, onHelpAbout)
        COMMAND_ID_HANDLER_EX(ID_PANE_CLOSE, onPaneClose)

        CHAIN_MSG_MAP(CFrameWindowImpl<MainWindow>)
        CHAIN_MSG_MAP(CUpdateUI<MainWindow>)
        CHAIN_MSG_MAP(TabletDetecter<MainWindow>)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BEGIN_UPDATE_UI_MAP(MainWindow)
        UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
        UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
        UPDATE_ELEMENT(ID_VIEW_LOGS, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
    END_UPDATE_UI_MAP()

    MainWindow();
    virtual ~MainWindow() = default;

    BOOL PreTranslateMessage(MSG* pMsg);
    BOOL OnIdle();

protected:
    virtual void onTabletConnected(PCWSTR szDevicePath) override;
    virtual void onTabletDisconnected(PCWSTR szDevicePath) override;

private:
    LogListView         _logList;
    CHorSplitterWindow  _mainSplitter;
    CanvasView          _canvasView;
    DetailView          _detailView;
    CSplitterWindow     _clientSplitter;

    TabletReader*       _pReader;

    int                 onCreate(LPCREATESTRUCT pCreateStruct);
    void                onDestroy();

    void                onFileExit(UINT, int, CWindow);
    void                onViewToolbar(UINT, int, CWindow);
    void                onViewStatusBar(UINT, int, CWindow);
    void                onViewLogs(UINT, int, CWindow);
    void                onHelpAbout(UINT, int, CWindow);
    void                onPaneClose(UINT, int, CWindow);

    void                createChildren();
    void                onInitialUpdate();
    void                showLogs(bool show);
};