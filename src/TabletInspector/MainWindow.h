#pragma once


#include <QtWidgets>
#include "TabletDetector.h"
#include "TabletReader.h"
#include "ConnectionIndicator.h"


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
        fileExit = 101,

        viewToolBar = 201,
        viewStatusBar = 202,

        toolSettings = 301,

        helpAbout = 1101,
    };
    QMap<Actions, QAction*> _actions;
    TabletDetector*         _tabletDetector;
    TabletReader*           _tabletReader;
    ConnectionIndicator*    _connectionIndicator;

    void createActions();
    void createMenuBar();
    void createStatusBar();
    void createToolBar();
    void createSideBars();
    void createCentral();

    QAction*    createAction(Actions key, const QString text, void (MainWindow::* slot)(),
        QIcon icon = QIcon(), bool checkable = false);
    QDockWidget* createDock(const QString& title,
        QWidget* widget,
        Qt::DockWidgetArea* initialArea);
    void        onInitialUpdate();
    void        stopReader();

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
