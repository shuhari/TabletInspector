#pragma once


#include "resource.h"
#include "TabletInfo.h"


class TabletInfoPage : public CDialogImpl<TabletInfoPage>,
    public CWinDataExchange<TabletInfoPage> {
public:
    enum { IDD = IDD_TABLET_INFO_PAGE };

    BEGIN_MSG_MAP_EX(TabletInfoPage)
    END_MSG_MAP()

    BEGIN_DDX_MAP(TabletInfoPage)
        DDX_CONTROL_HANDLE(IDC_TABLET_PROPGRID, _lsv)
    END_DDX_MAP()

    TabletInfoPage();
    virtual ~TabletInfoPage() = default;

    void onInitialUpdate();
    void setInfo(TabletInfo* pInfo);

private:
    CListViewCtrl _lsv;

    void addItem(PCWSTR name, PCWSTR value);
    void addItem(PCWSTR name, UINT value);
};