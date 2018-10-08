#include "stdafx.h"
#include "HexPage.h"
#include "Strings.h"
#include "MainWindow.h"
#include "AnalyticsDialog.h"


HexPage::HexPage(QWidget* parent) :
    QListView(parent) {

    setFont(QFont("Courier New", 10));
    setUniformItemSizes(true);
    setContextMenuPolicy(Qt::ActionsContextMenu);

    _acAnalytics = new QAction(Strings::analytics(), this);
    connect(_acAnalytics, &QAction::triggered, this, &HexPage::onAnalytics);
    addAction(_acAnalytics);
}


HexPage::~HexPage() {
}


PenDataModel* HexPage::dataModel() {
    return qobject_cast<PenDataModel*>(model());
}


void HexPage::notifyTablet(TabletInfo* info) {
    _info = info;
}


void HexPage::notifyTabletData(const QByteArray& data) {
    dataModel()->add(data);
    scrollToBottom();
}


void HexPage::clearTabletData() {
    auto model = dataModel();
    model->clear();
}


QMainWindow* HexPage::getMainWindow() {
    QObject* parentObj = parent();
    while (parentObj) {
        QMainWindow* mainWin = qobject_cast<QMainWindow*>(parentObj);
        if (mainWin)
            return mainWin;
        parentObj = parentObj->parent();
    }
    return nullptr;
}


void HexPage::onAnalytics() {
    auto model = dataModel();
    if (!_info || !model)
        return;
    QModelIndex index = currentIndex();
    if (index.row() < 0 && index.row() >= model->rowCount())
        return;

    auto data = model->data(index, Qt::UserRole).toByteArray();
    auto dataType = DataParser(data).dataType();
    if (dataType == DataParser::PenDown) {
        auto data = model->data(index).toByteArray();
        int nMin = 0, nMax = 0;
        if (model->getAnalyticsRange(index, nMin, nMax)) {
            MainWindow* mainWin = qobject_cast<MainWindow*>(getMainWindow());
            AnalyticsDialog dlg(_info, mainWin->dataModel(), nMin, nMax, mainWin);
            dlg.exec();
        }
    }
}