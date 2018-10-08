#include "stdafx.h"
#include "HexPage.h"



HexPage::HexPage(QWidget* parent) :
    QListView(parent) {

    setFont(QFont("Courier New", 10));
    this->setUniformItemSizes(true);
}


HexPage::~HexPage() {
}


PenDataModel* HexPage::dataModel() {
    return qobject_cast<PenDataModel*>(model());
}


void HexPage::notifyTablet(TabletInfo* info) {
}


void HexPage::notifyTabletData(const QByteArray& data) {
    dataModel()->add(data);
    scrollToBottom();
}


void HexPage::clearTabletData() {
    auto model = dataModel();
    model->clear();
}