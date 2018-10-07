#pragma once


#include "Public.h"


class RealTimePage : public QWidget {
    Q_OBJECT
public:
    RealTimePage(QWidget* parent = nullptr);
    virtual ~RealTimePage() = default;

    void setInfo(TabletInfo* info);
    void setData(const QByteArray& data);

private:
    TabletInfo*     _tabletInfo;
    QLabel*         _lblXValue;
    QLabel*         _lblYValue;
    QLabel*         _lblPressureValue;
    QProgressBar*   _pressureProgress;

    void setupUi();
};

