#pragma once


#include "stdafx.h"


enum ImageKey {
    app = 1, settings = 2,
    
    prop = 11, data = 12, logs = 13,

    debug = 21, info = 22, warn = 23, error = 24,

    connected = 31, disconnected = 32,
};


class ImageRegistry {
public:
    static QIcon icon(ImageKey key);
    static QPixmap pixmap(ImageKey key);

private:
    ImageRegistry();
    QString getPath(ImageKey key);
    void define(ImageKey key, PCSTR path);

    static ImageRegistry _instance;
    QMap<ImageKey, QString> _paths;
};