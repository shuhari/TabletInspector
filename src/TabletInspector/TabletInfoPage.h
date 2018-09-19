#pragma once


#include "resource.h"
#include "TabletInfo.h"
#include "Messages.h"


class TabletInfoPage : public CDialogImpl<TabletInfoPage>,
    public CWinDataExchange<TabletInfoPage> {
public:
    enum { IDD = IDD_TABLET_INFO_PAGE };

    BEGIN_MSG_MAP_EX(TabletInfoPage)
    END_MSG_MAP()

    BEGIN_DDX_MAP(TabletInfoPage)
        DDX_CONTROL_HANDLE(IDC_TABLET_PROPGRID, _lsv)
        DDX_CONTROL_HANDLE(IDC_X, _lblX)
        DDX_CONTROL_HANDLE(IDC_Y, _lblY)
        DDX_CONTROL_HANDLE(IDC_PRESSURE, _lblPressure)
        DDX_CONTROL_HANDLE(IDC_PRESSURE_PROGRESS, _presureProgress)
    END_DDX_MAP()

    TabletInfoPage();
    virtual ~TabletInfoPage() = default;

    void onInitialUpdate();
    void setInfo(TabletInfo* pInfo);
    void setPacket(PacketDataMessage* pMsg);

private:
    CListViewCtrl       _lsv;
    CStatic             _lblX;
    CStatic             _lblY;
    CStatic             _lblPressure;
    CProgressBarCtrl    _presureProgress;

    TabletInfo*         _pTabletInfo;

    void addItem(PCWSTR name, PCWSTR value);
    void addItem(PCWSTR name, UINT value);
};