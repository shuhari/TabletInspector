#pragma once


#include "Public.h"
#include "QtHelper.h"



class RealTimePage : public QWidget,
    public ITabletAwareWidget {
    Q_OBJECT
public:
    RealTimePage(QWidget* parent = nullptr);
    virtual ~RealTimePage();

    void notifyTablet(TabletInfo* info) override;
    void notifyTabletData(const QByteArray& data) override;
    void clearTabletData() override;

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
    void            updateTouch(int touchBtnIndex);
    void            updateSlider(int sliderId);
    void            updatePen(DataParser& parser);
};

