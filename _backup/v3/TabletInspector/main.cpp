#include "stdafx.h"
#include "App.h"
#include "MainWindow.h"


int main(int argc, char *argv[])  {
    App app(argc, argv);

    MainWindow mainWin;
    mainWin.show();

    return app.exec();
}
