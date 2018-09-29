#pragma once

#include <QtWidgets/QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow() = default;

private:
    enum Actions {
        fileExit = 101,

        viewToolBar = 201,
        viewStatusBar = 202,

        toolSettings = 301,

        helpAbout = 1101,
    };
    QMap<Actions, QAction*> _actions;

    void createActions();
    void createMenuBar();
    void createStatusBar();
    void createToolBar();
    void createSideBars();
    void createCentral();

    QAction* createAction(Actions key, const QString text, void (MainWindow::* slot)(),
        QIcon icon = QIcon(), bool checkable = false);
    QDockWidget* createDock(const QString& title,
        QWidget* widget,
        Qt::DockWidgetArea* initialArea);

private slots:
    void        onFileExit();
    void        onViewStatusBar();
    void        onToolSettings();
    void        onHelpAbout();
};
