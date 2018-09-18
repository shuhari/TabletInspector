#include "stdafx.h"
#include "LogListView.h"
#include "resource.h"


using namespace std;


#define ICON_DEBUG              0
#define ICON_INFO               1
#define ICON_WARN               2
#define ICON_ERROR              3
#define MAX_ITEMS               1000


#define FORMAT_CONTENT()              \
    CString sFormat;                  \
    sFormat.LoadString(uIdStr);       \
    WCHAR szContent[1024] = { 0 };    \
    va_list args;                     \
    va_start(args, uIdStr);           \
    _vsnwprintf_s(szContent, _countof(szContent), _TRUNCATE, sFormat, args); \
    va_end(args)                     \


LogListView::LogListView() {
}


void LogListView::onInitialUpdate() {
    BOOL bRet = _iml.Create(IDB_LOGLIST, 16, 1, RGB(0, 0x7F, 0x46));
    ATLASSERT(bRet);
    SetImageList(_iml, LVSIL_SMALL);

    SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    vector<LVCOLUMN> columns;
    addColumnsFromResStr(IDS_LOGLIST_COLUMNS);

    // debug(IDS_LOG_TABLET_CONNECTED, L"abc");
    // info(IDS_LOG_TABLET_CONNECTED, L"abc");
    // warn(IDS_LOG_TABLET_CONNECTED, L"abc");
    // error(IDS_LOG_TABLET_CONNECTED, L"abc");
}


void LogListView::onDestroy() {
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


void LogListView::debug(UINT uIdStr, ...) {
    FORMAT_CONTENT();
    addLog(ICON_DEBUG, szContent);
}


void LogListView::info(UINT uIdStr, ...) {
    FORMAT_CONTENT();
    addLog(ICON_INFO, szContent);
}


void LogListView::warn(UINT uIdStr, ...) {
    FORMAT_CONTENT();
    addLog(ICON_WARN, szContent);
}


void LogListView::error(UINT uIdStr, ...) {
    FORMAT_CONTENT();
    addLog(ICON_ERROR, szContent);
}


void LogListView::addLog(int nIcon, PCWSTR szContent) {
    if (GetItemCount() >= MAX_ITEMS)
        DeleteItem(0);

    SYSTEMTIME st;
    GetLocalTime(&st);
    CString sTime;
    sTime.Format(L"%02d:%02d:%02d",
        st.wHour, st.wMinute, st.wSecond);

    int nIndex = GetItemCount();
    InsertItem(nIndex, sTime, nIcon);
    SetItemText(nIndex, 1, szContent);
    EnsureVisible(nIndex, TRUE);
}
