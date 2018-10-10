#pragma once


#include "Public.h"
#include "Models.h"
#include "Canvas.h"


class AnalyticsDialog : public QDialog {
    Q_OBJECT
public:
    AnalyticsDialog(TabletInfo* info, PenDataModel* model, int nMin, int nMax, QWidget* parent = nullptr);
    virtual ~AnalyticsDialog();

private:
    TabletInfo          _tabletInfo;
    PenDataTableModel*  _model;
    QTableView*         _table;
    Canvas*             _canvas;
    QChartView*         _pressureView;
    QChartView*         _altitudeView;
    QChartView*         _azimuthView;

    void                setupUi();
    void                loadData();
    QChartView*         createChartView(const QString& title);
    void                setLineSeries(QChartView* view, QLineSeries* series);
};

