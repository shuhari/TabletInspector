#include "stdafx.h"
#include "TabletInfoPage.h"


TabletInfoPage::TabletInfoPage(QWidget* parent) :
    QTableWidget(parent) {

    setColumnCount(2);
    horizontalHeader()->hide();
    verticalHeader()->hide();
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    setInfo(nullptr);
}


void TabletInfoPage::setInfo(TabletInfo* pInfo) {
    if (pInfo) {
        setRowCount(14);
        int iRow = 0;
        setRow(iRow++, tr("OEM"), pInfo->oem);
        setRow(iRow++, tr("Model"), pInfo->model);
        setRow(iRow++, tr("Fixture Date"), pInfo->fixtureDate);
        setRow(iRow++, tr("Company"), pInfo->company);
        setRow(iRow++, tr("Width"), QString("%1").arg(pInfo->size.width()));
        setRow(iRow++, tr("Height"), QString("%1").arg(pInfo->size.height()));
        setRow(iRow++, tr("Max Pressure"), QString("%1").arg(pInfo->maxPressure));
        setRow(iRow++, "pbtnNum", QString("%1").arg(pInfo->pbtnNum));
        setRow(iRow++, "hbtnNum", QString("%1").arg(pInfo->hbtnNum));
        setRow(iRow++, "sbtnNum", QString("%1").arg(pInfo->sbtnNum));
        setRow(iRow++, "LPI", QString("%1").arg(pInfo->lpi));
        setRow(iRow++, tr("Rate"), QString("%1").arg(pInfo->rate));
        setRow(iRow++, tr("IsMonitor"), QString("%1").arg(pInfo->isMonitor));
        setRow(iRow++, tr("IsPassive"), QString("%1").arg(pInfo->isPassive));
        resizeColumnsToContents();
    }
    else {
        setRowCount(0);
    }
}


void TabletInfoPage::setRow(int nRow, const QString& name, const QString& value) {
    setCell(nRow, 0, name);
    setCell(nRow, 1, value);
}


void TabletInfoPage::setCell(int nRow, int nCol, const QString& text) {
    auto pItem = item(nRow, nCol);
    if (!pItem) {
        pItem = new QTableWidgetItem();
        setItem(nRow, nCol, pItem);
        pItem->setText(text);
    }
}
