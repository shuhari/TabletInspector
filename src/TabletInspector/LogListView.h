#pragma once


#include <vector>


typedef CWinTraitsOR<WS_CHILD | WS_VISIBLE | 
    LVS_REPORT | LVS_SINGLESEL | LVS_NOSORTHEADER> LogListTraits;


class LogListView : public CWindowImpl<LogListView, CListViewCtrl, LogListTraits> {
public:
    DECLARE_WND_SUPERCLASS(NULL, WC_LISTVIEW)

    BEGIN_MSG_MAP_EX(LogListView)
        MSG_WM_DESTROY(onDestroy)
        DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()

    LogListView();
    virtual ~LogListView() = default;

    void onInitialUpdate();

private:
    CImageList      _iml;

    void            onDestroy();

    void            addColumnsFromResStr(UINT uStrId);
};

