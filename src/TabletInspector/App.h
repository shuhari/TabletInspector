#pragma once


#include "AppSettings.h"


class App : public QApplication {
    Q_OBJECT
public:
    App(int argc, char* argv[]);
    virtual ~App() = default;

    inline AppSettings* settings() { return _settings; }

private:
    AppSettings*    _settings;

    QString         expandPath(const QString& fileName);
    bool            loadTranslationFile(const QString& fileName);
};

