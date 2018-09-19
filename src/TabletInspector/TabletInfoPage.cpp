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
        addItem(L"Company", pInfo->company);
        addItem(L"MaxX", pInfo->size.cx);
        addItem(L"MaxY", pInfo->size.cy);
        addItem(L"MaxPressure", pInfo->maxPressure);
        addItem(L"PBtnNum", pInfo->pbtnNum);
        addItem(L"HBtnNum", pInfo->hbtnNum);
        addItem(L"SBtnNum", pInfo->sbtnNum);
        addItem(L"LPI", pInfo->lpi);
        addItem(L"Rate", pInfo->rate);
        addItem(L"IsMonitor", pInfo->isMonitor);
        addItem(L"IsPassive", pInfo->isPassive);
    }
}


void TabletInfoPage::addItem(PCWSTR name, PCWSTR value) {
    int nIndex = _lsv.GetItemCount();
    _lsv.InsertItem(nIndex, name);
    _lsv.SetItemText(nIndex, 1, value);
}


void TabletInfoPage::addItem(PCWSTR name, UINT value) {
    CString sValue;
    sValue.Format(L"%u", value);
    addItem(name, sValue);
}