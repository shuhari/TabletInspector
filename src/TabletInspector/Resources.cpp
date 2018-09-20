#include "stdafx.h"
#include "Resources.h"


ImageRegistry ImageRegistry::_instance;


ImageRegistry::ImageRegistry() {
    define(ImageKey::app, ":/images/app.ico");

    define(ImageKey::prop, ":/images/prop.png");
    define(ImageKey::data, ":/images/data.png");
    define(ImageKey::logs, ":/images/logs.png");

    define(ImageKey::debug, ":/images/debug.png");
    define(ImageKey::info, ":/images/info.png");
    define(ImageKey::warn, ":/images/warn.png");
    define(ImageKey::error, ":/images/error.png");

    define(ImageKey::connected, ":/images/connected.png");
    define(ImageKey::disconnected, ":/images/disconnected.png");
}


void ImageRegistry::define(ImageKey key, PCSTR path) {
    _paths[key] = QString(path);
}


QString ImageRegistry::getPath(ImageKey key) {
    if (_paths.contains(key))
        return _paths[key];
    return QString("");
}


QIcon ImageRegistry::icon(ImageKey key) {
    auto path = _instance.getPath(key);
    return path.isEmpty() ? QIcon() : QIcon(path);
}

QPixmap ImageRegistry::pixmap(ImageKey key) {
    auto path = _instance.getPath(key);
    return path.isEmpty() ? QPixmap() : QPixmap(path);
}
