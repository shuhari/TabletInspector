#pragma once


#include "resource.h"
#include "LogListView.h"


class MainWindow : public CFrameWindowImpl<MainWindow> {
public:
    DECLARE_FRAME_WND_CLASS(L"TabletInspector_MainWindow", IDR_MAINFRAME)

    BEGIN_MSG_MAP_EX(MainWindow)
        MSG_WM_CREATE(onCreate)
        MSG_WM_DESTROY(onDestroy)

        COMMAND_ID_HANDLER_EX(ID_FILE_EXIT, onFileExit)
        COMMAND_ID_HANDLER_EX(ID_HELP_ABOUT, onHelpAbout)

        CHAIN_MSG_MAP(CFrameWindowImpl<MainWindow>)
    END_MSG_MAP()

    MainWindow();
    virtual ~MainWindow() = default;

private:
    LogListView         _logList;
    CHorSplitterWindow  _mainSplitter;
    CPaneContainer      _logListContainer;

    int                 onCreate(LPCREATESTRUCT pCreateStruct);
    void                onDestroy();

    void                onFileExit(UINT, int, CWindow);
    void                onHelpAbout(UINT, int, CWindow);

    void                createChildren();
    void                onInitialUpdate();
};