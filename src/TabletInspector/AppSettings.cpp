#include "stdafx.h"
#include "AppSettings.h"



#define GROUP_GENERAL               "generic"

#define ITEM_LANGUAGE               "language"


AppSettings::AppSettings(const QString& fileName, QObject* parent) 
    : QSettings(fileName, IniFormat, parent) {
}




void AppSettings::load() {
    beginGroup(GROUP_GENERAL);
    _language = value(ITEM_LANGUAGE, "").toString();
    endGroup();
}


void AppSettings::save() {
    beginGroup(GROUP_GENERAL);
    setValue(ITEM_LANGUAGE, _language);
    endGroup();

    sync();
}
