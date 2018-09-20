#pragma once


#include "stdafx.h"
#include "CanvasWidget.h"
#include "ConnectionIndicator.h"
#include "LogList.h"
#include "UsbDetector.h"


class MainWindow : public QMainWindow, public QAbstractNativeEventFilter
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    virtual ~MainWindow() = default;

    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override;

// override nativeEvent() caught wrong message. Don't know why, but inherit QAbstractNativeEventFilter works.
// protected:
    // virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

private:
    enum Actions {
        fileExit = 1,
        viewToolBar = 11, viewStatusBar = 12, viewProp = 13, viewData = 14, viewLogs = 15,
        helpAbout = 21,
    };
    QMap<Actions, QAction*> _actions;
    
    CanvasWidget*           _canvas;
    ConnectionIndicator*    _connectionIndicator;
    LogList*                _logList;

    UsbDetector*            _usbDetector;

    void createActions();
    void createToolBar();
    void createStatusBar();
    void createDocks();
    void createCentral();
    void createMenuBar();

    QDockWidget*    newDock(const QString& title, QWidget* widget, Qt::DockWidgetArea area);

private slots:
    void        onFileExit();
    void        onViewStatusBar();
    void        onHelpAbout();

    void        onDeviceConnected(const QString& devicePath);
    void        onDeviceDisconnected(const QString& devicePath);
};
