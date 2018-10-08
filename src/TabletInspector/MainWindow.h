#pragma once


#include "Public.h"
#include "Models.h"
#include "TabletDetector.h"
#include "TabletReader.h"
#include "ConnectionIndicator.h"
#include "LogList.h"
#include "Canvas.h"
#include "TabletInfoPage.h"
#include "HexPage.h"
#include "RealTimePage.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow() = default;

    inline PenDataModel* dataModel() { return _penDataModel; }

protected:
    virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
    virtual void closeEvent(QCloseEvent* event) override;

private:
    enum Actions {
        None = 0,

        fileExit = 101,

        editClear = 201,

        viewToolBar = 301,
        viewStatusBar = 302,
        viewLogs = 303,
        viewProp = 304,
        viewData = 305,

        toolSettings = 401,

        helpAbout = 501,
    };
    QMap<Actions, QAction*> _actions;
    TabletDetector*         _tabletDetector;
    TabletReader*           _tabletReader;
    PenDataModel*           _penDataModel;

    ConnectionIndicator*    _connectionIndicator;
    LogList*                _logList;
    Canvas*                 _canvas;
    TabletInfoPage*         _infoPage;
    RealTimePage*           _realTimePage;
    HexPage*                _hexPage;

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
    void        getTabletAwareWidgets(QList<ITabletAwareWidget*>& widgets);

private slots:
    void        onFileExit();
    void        onEditClear();
    void        onViewStatusBar();
    void        onToolSettings();
    void        onHelpAbout();

    void        onTabletConnected(const QString& devicePath);
    void        onTabletDisconnected(const QString& devicePath);
    void        onTabletReadData(const QByteArray& buffer);
    void        onTabletReadError(DWORD dwError);
};
