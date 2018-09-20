#include "stdafx.h"
#include "Resources.h"


ImageRegistry ImageRegistry::_instance;


ImageRegistry::ImageRegistry() {
    _items[ImageKey::app] = ":/images/app.ico";

    _items[ImageKey::prop] = ":/images/prop.png";
    _items[ImageKey::data] = ":/images/data.png";
    _items[ImageKey::logs] = ":/images/logs.png";

    _items[ImageKey::connected] = ":/images/connected.png";
    _items[ImageKey::disconnected] = ":/images/disconnected.png";
}


QString ImageRegistry::getPath(ImageKey key) {
    if (_items.contains(key))
        return _items[key];
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
