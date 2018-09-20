#pragma once


#include "stdafx.h"
#include "AppSettings.h"


class App : public QApplication {
    Q_OBJECT

public:
    App(int argc, char* argv[]);
    virtual ~App();

    inline AppSettings& settings() { return *_settings; }

    QStringList enumLocales();

private:
    AppSettings*       _settings;

    QString             expandPath(const QString& suffix);
    void                applyTranslation(const QString& localeName);
};
