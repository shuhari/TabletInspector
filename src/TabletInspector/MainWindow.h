#pragma once


#include "stdafx.h"
#include "CanvasWidget.h"
#include "ConnectionIndicator.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    virtual ~MainWindow() = default;

private:
    enum Actions {
        fileExit = 1,
        viewToolBar = 11, viewStatusBar = 12, viewProp = 13, viewData = 14, viewLogs = 15,
        helpAbout = 21,
    };
    QMap<Actions, QAction*> _actions;
    CanvasWidget*           _canvas;
    ConnectionIndicator*    _connectionIndicator;

    void createActions();
    void createToolBar();
    void createStatusBar();
    void createDocks();
    void createCentral();
    void createMenuBar();

    QDockWidget* newDock(const QString& title, QWidget* widget, Qt::DockWidgetArea area);

private slots:
    void        onFileExit();
    void        onViewStatusBar();
    void        onHelpAbout();
};
