#pragma once


#include "precompiled.h"


class TabletIndicatorWidget : public QWidget {
    Q_OBJECT
public:
    TabletIndicatorWidget();
    virtual ~TabletIndicatorWidget() = default;

    void setConnected(const QString& name);
    void setDisconnected();

private:
    QLabel* _icon;
    QLabel* _text;
};

