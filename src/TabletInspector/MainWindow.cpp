#include "stdafx.h"
#include "MainWindow.h"
#include "Strings.h"
#include "Resources.h"
#include "QtHelper.h"
#include "SettingsDialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)  {

    setWindowTitle(Strings::appTitle());
    setWindowIcon(Resources::appIcon());
    setMinimumSize(800, 600);

    createActions();
    createToolBar();
    createStatusBar();
    createSideBars();
    createCentral();
    createMenuBar();
}


QAction* MainWindow::createAction(Actions key, const QString text, void (MainWindow::* slot)(),
    QIcon icon/*=QIcon()*/, bool checkable/*=false*/) {
    QAction* action = new QAction(text);
    _actions[key] = action;
    connect(action, &QAction::triggered, this, slot);

    if (!icon.isNull())
        action->setIcon(icon);
    action->setCheckable(checkable);

    return action;
}


void MainWindow::createActions() {
    createAction(fileExit, Strings::actionFileExit(), &MainWindow::onFileExit);

    createAction(viewStatusBar, Strings::actionViewStatusBar(), &MainWindow::onViewStatusBar);

    createAction(toolSettings, Strings::actionToolSettings(), &MainWindow::onToolSettings,
        Resources::settings());

    createAction(helpAbout, Strings::actionHelpAbout(), &MainWindow::onHelpAbout);
}


void MainWindow::createToolBar() {
    auto toolbar = addToolBar(Strings::actionViewToolBar());
    
    toolbar->addSeparator();
    toolbar->addAction(_actions[toolSettings]);

    _actions[viewToolBar] = toolbar->toggleViewAction();
}


void MainWindow::createStatusBar() {
    auto status = statusBar();
}


void MainWindow::createSideBars() {

}


void MainWindow::createCentral() {

}

void MainWindow::createMenuBar() {
    auto mainMenu = menuBar();

    SubMenu(mainMenu, Strings::menuFile())
        .action(_actions[fileExit]);

    SubMenu(mainMenu, Strings::menuView())
        .action(_actions[viewToolBar])
        .action(_actions[viewStatusBar])
        .separator();

    SubMenu(mainMenu, Strings::menuTool())
        .action(_actions[toolSettings]);

    SubMenu(mainMenu, Strings::menuHelp())
        .action(_actions[helpAbout]);
}


void MainWindow::onFileExit() {
    close();
}


void MainWindow::onViewStatusBar() {
    // show/Hide status bar
    auto status = statusBar();
    bool visible = !status->isVisible();
    status->setVisible(visible);
    _actions[viewStatusBar]->setChecked(visible);
}


void MainWindow::onToolSettings() {
    SettingsDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
    }
}


void MainWindow::onHelpAbout() {

}


