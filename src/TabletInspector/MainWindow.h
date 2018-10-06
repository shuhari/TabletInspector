#pragma once


#include "TabletDetector.h"
#include "TabletReader.h"
#include "ConnectionIndicator.h"
#include "LogList.h"
#include "Canvas.h"
#include "TabletInfoPage.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow() = default;

protected:
    virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
    virtual void closeEvent(QCloseEvent* event) override;

private:
    enum Actions {
        None = 0,

        fileExit = 101,

        viewToolBar = 201,
        viewStatusBar = 202,
        viewLogs = 203,
        viewProp = 204,
        viewData = 205,

        toolSettings = 301,

        helpAbout = 1101,
    };
    QMap<Actions, QAction*> _actions;
    TabletDetector*         _tabletDetector;
    TabletReader*           _tabletReader;
    ConnectionIndicator*    _connectionIndicator;
    LogList*                _logList;
    Canvas*                 _canvas;
    TabletInfoPage*         _infoPage;

    void createActions();
    void createMenuBar();
    void createStatusBar();
    void createToolBar();
    void createSideBars();
    void createCentral();

    QAction*    createAction(Actions key, const QString text, void (MainWindow::* slot)(),
        QIcon icon = QIcon(), bool checkable = false);
    QDockWidget* createDock(const QString& title, QWidget* widget,
        Qt::DockWidgetAreas allowAreas, Qt::DockWidgetArea initialArea,
        Actions actionKey = Actions::None, QIcon actionIcon = QIcon());
    void        onInitialUpdate();
    void        stopReader(bool wait = false);

private slots:
    void        onFileExit();
    void        onViewStatusBar();
    void        onToolSettings();
    void        onHelpAbout();

    void        onTabletConnected(const QString& devicePath);
    void        onTabletDisconnected(const QString& devicePath);
    void        onTabletReadData(const QByteArray& buffer);
    void        onTabletReadError(DWORD dwError);
};
