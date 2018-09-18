#pragma once


#include "TabletInfoPage.h"
#include "TabletInfo.h"


class DetailView : public CTabViewImpl<DetailView> {
public:
    DECLARE_WND_CLASS_EX(L"TabletInspector_DetailView", 0, COLOR_APPWORKSPACE)

    DetailView();
    virtual ~DetailView() = default;

    void onInitialUpdate();
    void setTabletInfo(TabletInfo* pInfo);

private:
    TabletInfoPage          _tabletInfoPage;
};