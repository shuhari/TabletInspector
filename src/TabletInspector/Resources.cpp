#include "stdafx.h"
#include "Resources.h"


#define DEFINE_ICON(key, path) \
    QIcon Resources::key() { return QIcon(path); }


DEFINE_ICON(appIcon, ":/images/app.ico")
DEFINE_ICON(settings, ":/images/settings.png")
