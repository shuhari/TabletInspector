#pragma once

#include "precompiled.h"
#include "loglistwidget.h"
#include "canvaswidget.h"
#include "propertywidget.h"
#include "tabletindicatorwidget.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QAction*        _acFileExit;
    QAction*        _acViewToolBar;
    QAction*        _acViewStatusBar;
    QAction*        _acViewProp;
    QAction*        _acViewData;
    QAction*        _acViewLogs;
    QAction*        _acHelpAbout;

    LogListWidget*          _logList;
    CanvasWidget*           _canvas;
    PropertyWidget*         _propWidget;
    TabletIndicatorWidget*  _indicator;

    void createActions();
    void createMenuBar();
    void createToolBar();
    void createStatusBar();
    void createDocks();
    void createCentral();

    QDockWidget* createDock(const QString& title, QWidget* widget,
                            Qt::DockWidgetArea area);

private slots:
    void    onFileExit();
    void    onViewStatusBar();
    void    onHelpAbout();
};


