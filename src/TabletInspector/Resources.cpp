#include "stdafx.h"
#include "Resources.h"


QIcon Resources::appIcon() { return QIcon(":/images/app.ico"); }
QIcon Resources::settings() { return QIcon(":/images/settings.png"); }
QIcon Resources::debug() { return QIcon(":/images/debug.png"); }
QIcon Resources::info() { return QIcon(":/images/info.png"); }
QIcon Resources::warn() { return QIcon(":/images/warn.png"); }
QIcon Resources::error() { return QIcon(":/images/error.png"); }
QIcon Resources::logs() { return QIcon(":/images/logs.png"); }
QIcon Resources::prop() { return QIcon(":/images/prop.png"); }
QIcon Resources::data() { return QIcon(":/images/data.png"); }
QIcon Resources::clear() { return QIcon(":/images/clear.png"); }

QPixmap Resources::connected() { return QPixmap(":/images/connected.png"); }
QPixmap Resources::disconnected() { return QPixmap(":/images/disconnected.png"); }
QPixmap Resources::appIconPixmap() { return QPixmap(":/images/app.ico"); }
