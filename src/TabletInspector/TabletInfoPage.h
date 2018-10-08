#pragma once


#include "Public.h"


class TabletInfoPage : public QTableWidget,
    public ITabletAwareWidget {
    Q_OBJECT
public:
    TabletInfoPage(QWidget* parent = nullptr);
    virtual ~TabletInfoPage() = default;

    void notifyTablet(TabletInfo* info) override;
    void notifyTabletData(const QByteArray& data) override;
    void clearTabletData() override;

private:
    void setRow(int row, const QString& name, const QString& value, bool alignRight = false);
    void setCell(int row, int col, const QString& text, int align = Qt::AlignLeft);
    QString formatInt(int value);
    QString formatBool(bool value);
};

