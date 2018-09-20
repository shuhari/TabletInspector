#pragma once


#include "precompiled.h"
#include "tabletinfowidget.h"


class PropertyWidget : public QTabWidget
{
    Q_OBJECT
public:
    PropertyWidget();
    virtual ~PropertyWidget() = default;

private:
    TabletInfoWidget* _pTabletInfoWidget;
};

