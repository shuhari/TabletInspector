#include "stdafx.h"
#include "AppSettings.h"


#define GROUP_GENERAL       "generic"

#define KEY_LANGUAGE        "language"


AppSettings::AppSettings(const QString& fileName, QObject* parent) :
    QSettings(fileName, IniFormat, parent) {
}


void AppSettings::load() {
    beginGroup(GROUP_GENERAL);
    _language = value(KEY_LANGUAGE, "").toString();
    endGroup();
}


void AppSettings::save() {
    beginGroup(GROUP_GENERAL);
    setValue(KEY_LANGUAGE, _language);
    endGroup();

    sync();
}