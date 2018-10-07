#include "stdafx.h"
#include "RealTimePage.h"
#include "QtHelper.h"
#include "Strings.h"


RealTimePage::RealTimePage(QWidget* parent) :
    QWidget(parent),
    _tabletInfo(nullptr) {

    setupUi();
}


void RealTimePage::setupUi() {
    QLabel* lblX = new QLabel(Strings::x());
    QLabel* lblY = new QLabel(Strings::y());
    QLabel* lblPressure = new QLabel(Strings::pressure());
    _lblXValue = new QLabel();
    _lblYValue = new QLabel();
    _lblPressureValue = new QLabel();
    _pressureProgress = new QProgressBar();
    _pressureProgress->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    auto grid = Layout::grid()
        .add(lblX, 0, 0)
        .add(_lblXValue, 0, 1)
        .add(lblY, 1, 0)
        .add(_lblYValue, 1, 1)
        .add(lblPressure, 2, 0)
        .add(_lblPressureValue, 2, 1)
        .add(_pressureProgress, 3, 0, 1, 2, Qt::AlignJustify)
        .layout();

    auto vbox = Layout::vbox()
        .add(grid)
        .addStretch(1)
        .apply(this);
}


void RealTimePage::setInfo(TabletInfo* info) {
    _tabletInfo = info;
    if (info) {
        _lblXValue->setText(QString("/ %1").arg(info->size.width()));
        _lblYValue->setText(QString("/ %1").arg(info->size.height()));
        _lblPressureValue->setText(QString("/ %1").arg(info->maxPressure));
        _pressureProgress->setRange(0, info->maxPressure);
    } else {
        _lblXValue->clear();
        _lblYValue->clear();
        _pressureProgress->setRange(0, 0);
        _pressureProgress->setValue(0);
    }
}


void RealTimePage::setData(const QByteArray& data) {
    if (!_tabletInfo || data.size() < 2 || data[0] != 0x08)
        return;
    BYTE sign = data[1];
    if ((sign & 0x80) != 0x80)
        return;
    int x = ((BYTE)(data[8]) << 16) | ((BYTE)(data[3]) << 8) | ((BYTE)data[2]);
    int y = ((BYTE)(data[9]) << 16) | ((BYTE)(data[5]) << 8) | ((BYTE)data[4]);
    int pressure = ((BYTE)(data[7]) << 8) | ((BYTE)data[6]);

    _lblXValue->setText(QString("%1 / %2")
        .arg(x).arg(_tabletInfo->size.width()));
    _lblYValue->setText(QString("%1 / %2")
        .arg(y).arg(_tabletInfo->size.height()));
    _lblPressureValue->setText(QString("%1 / %2")
        .arg(pressure).arg(_tabletInfo->maxPressure));
    _pressureProgress->setValue(pressure);
}
