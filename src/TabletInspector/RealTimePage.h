#pragma once


#include "Public.h"
#include "QtHelper.h"



class RealTimePage : public QWidget {
    Q_OBJECT
public:
    RealTimePage(QWidget* parent = nullptr);
    virtual ~RealTimePage();

    void setInfo(TabletInfo* info);
    void setData(const QByteArray& data);

private:
    TabletInfo*     _tabletInfo;
    LabelPair*      _xPair;
    LabelPair*      _yPair;
    LabelPair*      _pressurePair;
    LabelPair*      _penPair;
    LabelPair*      _penBtnPair;
    LabelPair*      _touchPair;
    LabelPair*      _sliderPair;
    QProgressBar*   _pressureProgress;

    void            setupUi();
    void            clearAll();
};

