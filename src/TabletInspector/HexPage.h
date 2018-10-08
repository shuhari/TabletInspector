#pragma once


#include "Public.h"
#include "Models.h"


class HexPage : public QListView,
    public ITabletAwareWidget {
    Q_OBJECT
public:
    HexPage(QWidget* parent = nullptr);
    virtual ~HexPage();

    void notifyTablet(TabletInfo* info) override;
    void notifyTabletData(const QByteArray& data) override;
    void clearTabletData() override;

private:
    QAction*        _acAnalytics;
    TabletInfo*     _info;
    PenDataModel*   dataModel();
    QMainWindow*    getMainWindow();

private slots:
    void            onAnalytics();
};

