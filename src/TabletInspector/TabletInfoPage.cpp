#include "stdafx.h"
#include "TabletInfoPage.h"


TabletInfoPage::TabletInfoPage() {
}


void TabletInfoPage::onInitialUpdate() {
    DoDataExchange(DDX_LOAD);
    _lsv.InsertColumn(0, L"Name", LVCFMT_LEFT, 200);
    _lsv.InsertColumn(1, L"Value", LVCFMT_LEFT, 600);
}


void TabletInfoPage::setInfo(TabletInfo* pInfo) {
    _lsv.DeleteAllItems();

    if (pInfo) {
        addItem(L"OEM", pInfo->oem);
        addItem(L"Model", pInfo->model);
        addItem(L"Fixture Date", pInfo->fixtureDate);
    }
}


void TabletInfoPage::addItem(PCWSTR name, PCWSTR value) {
    int nIndex = _lsv.GetItemCount();
    _lsv.InsertItem(nIndex, name);
    _lsv.SetItemText(nIndex, 1, value);
}