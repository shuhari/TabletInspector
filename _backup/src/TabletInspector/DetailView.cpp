#include "stdafx.h"
#include "DetailView.h"
#include "resource.h"


DetailView::DetailView() {
}


void DetailView::onInitialUpdate() {
    _tabletInfoPage.Create(m_hWnd);
    AddPage(_tabletInfoPage, L"Tablet Information");

    _tabletInfoPage.onInitialUpdate();
}


void DetailView::setTabletInfo(TabletInfo* pInfo) {
    _tabletInfoPage.setInfo(pInfo);
}

void DetailView::setPacketData(PacketDataMessage* pMsg) {
    _tabletInfoPage.setPacket(pMsg);
}
