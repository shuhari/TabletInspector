#pragma once


#include "Public.h"
#include "Models.h"


class TabletInfoPage : public QTableView,
    public ITabletAwareWidget {
    Q_OBJECT
public:
    TabletInfoPage(QWidget* parent = nullptr);
    virtual ~TabletInfoPage() = default;

    void notifyTablet(TabletInfo* info) override;
    void notifyTabletData(const QByteArray& data) override;
    void clearTabletData() override;

private:
    TabletInfoModel*        _model;
};

