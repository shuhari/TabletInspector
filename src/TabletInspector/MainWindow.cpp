#include "stdafx.h"
#include "MainWindow.h"


MainWindow::MainWindow() {
}


int MainWindow::onCreate(LPCREATESTRUCT pCreateStruct) {
    return 0;
}


void MainWindow::onDestroy() {
    PostQuitMessage(0);
}
