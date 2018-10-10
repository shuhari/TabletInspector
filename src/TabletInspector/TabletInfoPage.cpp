#include "stdafx.h"
#include "TabletInfoPage.h"
#include "Strings.h"


TabletInfoPage::TabletInfoPage(QWidget* parent) :
    QTableView(parent) {

    _model = new TabletInfoModel(this);
    setModel(_model);

    horizontalHeader()->hide();
    verticalHeader()->show();
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    verticalHeader()->setMinimumWidth(100);
    setShowGrid(false);
    setFont(QFont("Tahoma", 9));

    notifyTablet(nullptr);
}


void TabletInfoPage::notifyTablet(TabletInfo* info) {
    _model->setInfo(info);
    resizeColumnsToContents();
}


void TabletInfoPage::notifyTabletData(const QByteArray& data) {
}


void TabletInfoPage::clearTabletData() {
}


