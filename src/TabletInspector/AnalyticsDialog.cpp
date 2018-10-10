#include "stdafx.h"
#include "AnalyticsDialog.h"
#include "Strings.h"
#include "QtHelper.h"


AnalyticsDialog::AnalyticsDialog(TabletInfo* info, PenDataModel* model, int nMin, int nMax, QWidget* parent) 
    : QDialog(parent) {

    _tabletInfo.copyFrom(*info);
    _model = new PenDataTableModel(model, nMin, nMax, this);

    setWindowTitle(Strings::analytics());
    setupUi();
    loadData();
}


AnalyticsDialog::~AnalyticsDialog() {
}


void AnalyticsDialog::setupUi() {
    auto btnBox = new QDialogButtonBox(QDialogButtonBox::Cancel);

    _table = new QTableView();
    _table->setFixedWidth(600);
    _table->setMinimumHeight(200);
    _table->setSelectionBehavior(QAbstractItemView::SelectRows);
    _table->setSelectionMode(QAbstractItemView::SingleSelection);
    _table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _table->setFont(QFont("Courier New", 9));

    _canvas = new Canvas();
    _canvas->notifyTablet(&_tabletInfo);
    _canvas->setMinimumSize(200, 100);

    _pressureView = createChartView(Strings::pressure());
    _altitudeView = createChartView(Strings::altitude());
    _azimuthView = createChartView(Strings::azimuth());

    auto grid = Layout::grid()
        .add(_canvas, 0, 0)
        .add(_pressureView, 0, 1)
        .add(_altitudeView, 1, 0)
        .add(_azimuthView, 1, 1);

    auto hbox = Layout::hbox()
        .add(_table)
        .add(grid.layout(), 1);

    Layout::vbox()
        .add(hbox.layout(), 1)
        .add(btnBox)
        .apply(this);

    connect(btnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}



QChartView* AnalyticsDialog::createChartView(const QString& title) {
    auto chart = new QChart();
    chart->setTitle(title);
    auto view = new QChartView(chart);
    view->setFrameStyle(QFrame::Panel);
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    return view;
}


void AnalyticsDialog::loadData() {
    _table->setModel(_model);

    auto pressureSeries = new QLineSeries();
    auto altitudeSeries = new QLineSeries();
    auto azimuthSeries = new QLineSeries();

    for (int i = 0; i < _model->rowCount(); i++) {
        const QByteArray& data = _model->at(i);
        DataParser parser(data);
        QPoint pos = parser.position();
        int pressure = parser.pressure();
        QPoint tilt = parser.tilt();
        QPoint polar = parser.convertToPolar(tilt);

        pressureSeries->append(i + 1, pressure);
        altitudeSeries->append(i + 1, polar.x());
        azimuthSeries->append(i + 1, polar.y());
    }
    _table->resizeColumnsToContents();

    _canvas->loadData(_model);
    setLineSeries(_pressureView, pressureSeries);
    setLineSeries(_altitudeView, altitudeSeries);
    setLineSeries(_azimuthView, azimuthSeries);
}


void AnalyticsDialog::setLineSeries(QChartView* view, QLineSeries* series) {
    auto chart = view->chart();
    chart->addSeries(series);
    chart->createDefaultAxes();
}

