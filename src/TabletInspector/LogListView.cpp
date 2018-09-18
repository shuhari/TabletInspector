#include "stdafx.h"
#include "LogListView.h"
#include "resource.h"


using namespace std;


#define ICON_DEBUG              0
#define ICON_INFO               1
#define ICON_WARN               2
#define ICON_ERROR              3


LogListView::LogListView() {
}


void LogListView::onInitialUpdate() {
    _iml.Create(IDB_LOGLIST, 16, 0, RGB(0, 0, 0));
    SetImageList(_iml, LVSIL_NORMAL);

    SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    vector<LVCOLUMN> columns;
    addColumnsFromResStr(IDS_LOGLIST_COLUMNS);
}


void LogListView::onDestroy() {
    SetImageList(NULL, LVSIL_NORMAL);
    _iml.Destroy();
}


void LogListView::addColumnsFromResStr(UINT uStrId) {
    CString str;
    str.LoadStringW(uStrId);
    int iPos = 0;
    vector<CString> parts;
    while (true) {
        CString token = str.Tokenize(L",;", iPos);
        if (token.IsEmpty())
            break;
        parts.push_back(token);
    }
    size_t nColCount = parts.size() / 3;
    for (size_t i = 0; i < nColCount; i++) {
        LVCOLUMN col;
        memset(&col, 0, sizeof(col));
        col.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
        col.pszText = (PWSTR)(PCWSTR)parts[i * 3];
        CString formatStr = parts[i * 3 + 1];
        if (_wcsicmp(formatStr, L"center") == 0)
            col.fmt = LVCFMT_CENTER;
        else if (_wcsicmp(formatStr, L"right") == 0)
            col.fmt = LVCFMT_RIGHT;
        else
            col.fmt = LVCFMT_LEFT;
        col.cx = _wtoi(parts[i * 3 + 2]);
        InsertColumn((int)i, &col);
    }
}