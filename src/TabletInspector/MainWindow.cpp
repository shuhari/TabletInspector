#include "stdafx.h"
#include "MainWindow.h"
#include "Strings.h"
#include "Resources.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)  {

    setWindowTitle(Strings::appTitle());
    setWindowIcon(Resources::appIcon());
}
