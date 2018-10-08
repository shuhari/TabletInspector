#include "stdafx.h"
#include "TabletInfoPage.h"
#include "Strings.h"


TabletInfoPage::TabletInfoPage(QWidget* parent) :
    QTableWidget(parent) {

    setColumnCount(2);
    horizontalHeader()->hide();
    verticalHeader()->hide();
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setFont(QFont("Tahoma", 9));

    notifyTablet(nullptr);
}


void TabletInfoPage::notifyTablet(TabletInfo* info) {
    if (info) {
        setRowCount(13);
        int row = 0;
        QString size = QString("%1 x %2").arg(info->size.width())
            .arg(info->size.height());
        setRow(row++, Strings::oem(), info->oem);
        setRow(row++, Strings::model(), info->model);
        setRow(row++, Strings::fixtureDate(), info->fixtureDate);
        setRow(row++, Strings::company(), info->company);
        setRow(row++, Strings::size(), size, true);
        setRow(row++, Strings::maxPressure(), formatInt(info->maxPressure), true);
        setRow(row++, Strings::pBtnNum(), formatInt(info->pBtnNum), true);
        setRow(row++, Strings::hBtnNum(), formatInt(info->hBtnNum), true);
        setRow(row++, Strings::sBtnNum(), formatInt(info->sBtnNum), true);
        setRow(row++, Strings::lpi(), formatInt(info->lpi), true);
        setRow(row++, Strings::rate(), formatInt(info->rate), true);
        setRow(row++, Strings::isMonitor(), formatBool(info->isMonitor));
        setRow(row++, Strings::isPassive(), info->isPassive ? Strings::passive() :
            Strings::unPassive());
        resizeColumnsToContents();
    } else {
        setRowCount(0);
    }
}


void TabletInfoPage::notifyTabletData(const QByteArray& data) {
}


void TabletInfoPage::clearTabletData() {
}


void TabletInfoPage::setRow(int nRow, const QString& name, 
    const QString& value, bool alignRight/*=false*/) {
    setCell(nRow, 0, name);
    int align = alignRight ? Qt::AlignRight : Qt::AlignLeft;
    setCell(nRow, 1, value, align);
}


void TabletInfoPage::setCell(int nRow, int nCol, const QString& text, int align) {
    auto pItem = item(nRow, nCol);
    if (!pItem) {
        pItem = new QTableWidgetItem();
        setItem(nRow, nCol, pItem);
        pItem->setText(text);
        pItem->setTextAlignment(align);
    }
}


QString TabletInfoPage::formatInt(int value) {
    return QString("%1").arg(value);
}


QString TabletInfoPage::formatBool(bool value) {
    return value ? Strings::trueStr() : Strings::falseStr();
}
