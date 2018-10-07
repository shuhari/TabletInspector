#include "stdafx.h"
#include "RealTimePage.h"
#include "QtHelper.h"
#include "Strings.h"
#include "Public.h"


RealTimePage::RealTimePage(QWidget* parent) :
    QWidget(parent),
    _tabletInfo(nullptr) {

    setupUi();
}


RealTimePage::~RealTimePage() {
    delete _xPair;
    delete _yPair;
    delete _pressurePair;
    delete _penPair;
    delete _penBtnPair;
    delete _touchPair;
    delete _sliderPair;
}


void RealTimePage::setupUi() {
    _xPair = new LabelPair(Strings::x());
    _yPair = new LabelPair(Strings::y());
    _pressurePair = new LabelPair(Strings::pressure());
    _penPair = new LabelPair(Strings::pen());
    _penBtnPair = new LabelPair(Strings::penBtn());
    _touchPair = new LabelPair(Strings::touchBtn());
    _sliderPair = new LabelPair(Strings::slider());

    _pressureProgress = new QProgressBar();
    _pressureProgress->setTextVisible(false);
    _pressureProgress->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    auto grid = Layout::grid()
        .add(_xPair, 0, 0, Qt::AlignRight)
        .add(_yPair, 1, 0, Qt::AlignRight)
        .add(_pressurePair, 2, 0, Qt::AlignRight)
        .add(_pressureProgress, 3, 0, 1, 2, Qt::AlignJustify)
        .add(_penPair, 4, 0)
        .add(_penBtnPair, 5, 0)
        .add(_touchPair, 6, 0)
        .add(_sliderPair, 7, 0)
        .layout();

    auto vbox = Layout::vbox()
        .add(grid)
        .addStretch(1)
        .apply(this);
}


void RealTimePage::setInfo(TabletInfo* info) {
    _tabletInfo = info;
    if (info) {
        _xPair->setValue(QString("/ %1").arg(info->size.width()));
        _yPair->setValue(QString("/ %1").arg(info->size.height()));
        _pressurePair->setValue(QString("/ %1").arg(info->maxPressure));
        _pressureProgress->setRange(0, info->maxPressure);
    } else {
        _pressureProgress->setRange(0, 0);
        clearAll();
    }
}


void RealTimePage::clearAll() {
    _xPair->clearValue();
    _yPair->clearValue();
    _pressurePair->clearValue();
    _penPair->clearValue();
    _penBtnPair->clearValue();
    _touchPair->clearValue();
    _sliderPair->clearValue();

    _pressureProgress->setValue(0);
}


void RealTimePage::setData(const QByteArray& data) {

    clearAll();

    if (!_tabletInfo || data.size() < 2 || data[0] != 0x08)
        return;
    DataParser parser(data);
    if (parser.dataType() == DataParser::Unknown)
        return;

    auto dataType = parser.dataType();
    switch (dataType) {
        case DataParser::TouchBtn: {
            int nBtnIndex = parser.touchBtnIndex();
            if (nBtnIndex >= 0)
                _touchPair->setValue(Strings::touchBtnDown().arg(nBtnIndex + 1));
        }
                                   break;
        case DataParser::Slider: {
            int sliderId = parser.sliderId();
            _sliderPair->setValue(Strings::sliderPress().arg(sliderId));
        }
                                 break;

        case DataParser::PenDown:
        case DataParser::PenBtnDown:
        case DataParser::PenUp: {

            QPoint pos = parser.position();
            int pressure = parser.pressure();
            QString strX = QString("%1 / %2").arg(pos.x()).arg(_tabletInfo->size.width());
            QString strY = QString("%1 / %2").arg(pos.y()).arg(_tabletInfo->size.height());
            _xPair->setValue(strX);
            _yPair->setValue(strY);
            _pressurePair->setValue(QString("%1 / %2")
                .arg(pressure).arg(_tabletInfo->maxPressure));
            _pressureProgress->setValue(pressure);

            if (dataType == DataParser::PenDown) {
                _penPair->setValue(Strings::penDown());
            } else if (dataType == DataParser::PenBtnDown) {
                int nBtnIndex = parser.penBtnIndex();
                if (nBtnIndex >= 0) {
                    _penBtnPair->setValue(Strings::penBtnDown().arg(nBtnIndex + 1));
                }
            }
        }
        break;
    }
}
