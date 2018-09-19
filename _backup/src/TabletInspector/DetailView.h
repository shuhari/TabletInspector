#pragma once


#include "TabletInfoPage.h"
#include "TabletInfo.h"
#include "Messages.h"


class DetailView : public CTabViewImpl<DetailView> {
public:
    DECLARE_WND_CLASS_EX(L"TabletInspector_DetailView", 0, COLOR_APPWORKSPACE)

    DetailView();
    virtual ~DetailView() = default;

    void onInitialUpdate();
    void setTabletInfo(TabletInfo* pInfo);
    void setPacketData(PacketDataMessage* pMsg);

private:
    TabletInfoPage          _tabletInfoPage;
};