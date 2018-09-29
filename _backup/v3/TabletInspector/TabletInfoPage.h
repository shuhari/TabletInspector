#pragma once


#include "stdafx.h"
#include "Models.h"


class TabletInfoPage : public QTableWidget
{
    Q_OBJECT
public:
    TabletInfoPage(QWidget* parent = nullptr);
    virtual ~TabletInfoPage() = default;

    void setInfo(TabletInfo* pInfo = nullptr);

private:
    void setRow(int nRow, const QString& text, const QString& value);
    void setCell(int nRow, int nCol, const QString& text);
};

