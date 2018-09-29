#include "stdafx.h"
#include "App.h"


App::App(int argc, char* argv[])
    : QApplication(argc, argv) {

    // Read settings
    QString settingPath = expandPath("tabletinspector.ini");
    _settings = new AppSettings(settingPath, this);
    _settings->load();

    // load translations
    QString languageName = _settings->language();
    loadTranslationFile(QString("tabletinspector_%1.qm").arg(languageName));
}


QString App::expandPath(const QString& fileName) {
    QDir baseDir = QDir(applicationDirPath());
    return baseDir.absoluteFilePath(fileName);
}


bool App::loadTranslationFile(const QString& fileName) {
    QString filePath = expandPath(fileName);
    if (!QFile::exists(filePath)) {
        return false;
    }
    QTranslator* translator = new QTranslator(this);
    translator->load(filePath);
    return installTranslator(translator);
}
