#include "stdafx.h"
#include "App.h"


App::App(int argc, char* argv[])
    : QApplication(argc, argv) {

    QString settingPath = expandPath("tabletinspector.ini");
    _settings = new AppSettings(settingPath, this);
    _settings->load();

    applyTranslation(_settings->language());
}


App::~App() {
    // _settings->save();
    delete _settings;
    _settings = nullptr;
}


QString App::expandPath(const QString& suffix) {
    QDir baseDir = QDir(applicationDirPath());
    return baseDir.absoluteFilePath(suffix);
}


void App::applyTranslation(const QString& localeName) {
    QString fileName = QString("tabletinspector_%1.qm").arg(localeName);
    QString translationPath = expandPath(fileName);
    if (QFile::exists(translationPath)) {
        QTranslator* translator = new QTranslator(this);
        translator->load(translationPath);
        installTranslator(translator);
    }
}


QStringList App::enumLocales() {
    QStringList locales;
    locales << "en";
    locales << "zh";
    return locales;
}