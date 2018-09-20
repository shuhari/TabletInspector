#pragma once


#include "stdafx.h"


class App : public QApplication {
    Q_OBJECT

public:
    App(int argc, char* argv[]);
    virtual ~App() = default;
};
