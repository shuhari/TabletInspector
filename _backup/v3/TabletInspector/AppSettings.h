#pragma once


#include "stdafx.h"


class AppSettings : public QSettings
{
    Q_OBJECT
public:
    AppSettings(const QString& fileName, QObject* parent);
    virtual ~AppSettings() = default;

    inline QString language() { return _language; }

    inline void setLanguage(const QString& value) { _language = value; }

    void load();
    void save();

private:
    QString             _language;
};

