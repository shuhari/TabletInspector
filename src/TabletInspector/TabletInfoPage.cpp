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
    _pTabletInfo = pInfo;

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

        _presureProgress.SetRange(0, pInfo->maxPressure);
        _presureProgress.SetPos(0);
    }
    else {
        _lblX.SetWindowTextW(L"");
        _lblY.SetWindowTextW(L"");
        _lblPressure.SetWindowTextW(L"");
        _presureProgress.SetPos(0);
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

void TabletInfoPage::setPacket(PacketDataMessage* pMsg) {
    if (_pTabletInfo && pMsg && pMsg->isValid()) {
        static int prevX = 0, prevY = 0;
        static int prevPressure = 0;
        CPoint pt = pMsg->point();
        CString sText;
        if (pt.x != prevX) {
            sText.Format(L"X: %d / %d", pt.x, _pTabletInfo->size.cx);
            _lblX.SetWindowText(sText);
        }
        if (pt.y != prevY) {
            sText.Format(L"Y: %d / %d", pt.y, _pTabletInfo->size.cy);
            _lblY.SetWindowText(sText);
        }
        if (prevPressure != pMsg->pressure()) {
            sText.Format(L"Pressure: %d / %d", pMsg->pressure(), _pTabletInfo->maxPressure);
            _lblPressure.SetWindowTextW(sText);
            _presureProgress.SetPos(pMsg->pressure());
        }
        prevX = pt.x;
        prevY = pt.y;
        prevPressure = pMsg->pressure();
    }
}
