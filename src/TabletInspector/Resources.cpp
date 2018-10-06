#include "stdafx.h"
#include "Resources.h"


#define DEFINE_ICON(key, path) \
    QIcon Resources::key() { return QIcon(path); }

#define DEFINE_PIXMAP(key, path) \
    QPixmap Resources::key() { return QPixmap(path); }


DEFINE_ICON(appIcon, ":/images/app.ico")
DEFINE_ICON(settings, ":/images/settings.png")

DEFINE_PIXMAP(connected, ":/images/connected.png")
DEFINE_PIXMAP(disconnected, ":/images/disconnected.png")
