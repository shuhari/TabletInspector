#include "stdafx.h"
#include "Public.h"


// {62F12D4C-3431-4EFD-8DD7-8E9AAB18D30C}
GUID GUID_DEVINTERFACE_TABLET_WINUSB =
    { 0x62F12D4C, 0x3431, 0x4EFD, {0x8D, 0xD7, 0x8E, 0x9A, 0xAB, 0x18, 0xD3, 0x0C} };


QString TabletInfo::tabletName() {
    return QString("%1 %2").arg(oem).arg(model);
}


QString TabletInfo::sizeStr() {
    return QString("%1 x %2").arg(size.width())
        .arg(size.height());
}


void TabletInfo::copyFrom(TabletInfo& src) {
    devicePath = src.devicePath;
    oem = src.oem;
    model = src.model;
    fixtureDate = src.fixtureDate;
    company = src.company;
    size = src.size;
    maxPressure = src.maxPressure;
    pBtnNum = src.pBtnNum;
    sBtnNum = src.sBtnNum;
    hBtnNum = src.hBtnNum;
    lpi = src.lpi;
    rate = src.rate;
    isMonitor = src.isMonitor;
    isPassive = src.isPassive;
}


DataParser::DataParser(const QByteArray& data) :
    _data(data) {
    _dataType = getDataType();
}


DataParser::DataType DataParser::getDataType() {
    if (_data.size() < 2 || _data[0] != 0x08)
        return Unknown;

    BYTE sign = _data[1];
    if (sign == 0xE0) {
        return TouchBtn;
    }
    else if (sign == 0xF0) {
        return Slider;
    }
    else if ((sign & 0x80) == 0x80) {
        if ((sign & 0x81) == 0x81)
            return PenDown;
        if ((sign & 0x82) == 0x82 ||
            (sign & 0x84) == 0x84)
            return PenBtnDown;
        return PenUp;
    }
    return Unknown;
}


int DataParser::touchBtnIndex() {
    int touchId = ((BYTE)(_data[5]) << 8) | ((BYTE)_data[4]);
    return (int)log2(touchId);
}


int DataParser::sliderId() {
    return (BYTE)_data[5];
}


QPoint DataParser::position() {
    int x = ((BYTE)(_data[8]) << 16) | ((BYTE)(_data[3]) << 8) | ((BYTE)_data[2]);
    int y = ((BYTE)(_data[9]) << 16) | ((BYTE)(_data[5]) << 8) | ((BYTE)_data[4]);
    return QPoint(x, y);
}


int DataParser::pressure() {
    return ((BYTE)(_data[7]) << 8) | ((BYTE)_data[6]);
}


int DataParser::penBtnIndex() {
    BYTE sign = _data[1];
    if (sign == 0x82)
        return 0;
    else if (sign == 0x84)
        return 1;
    return -1;
}


QPoint DataParser::tilt() {
    BYTE ax = (BYTE)_data[10];
    BYTE ay = (BYTE)_data[11];
    return QPoint(ax, ay);
}


QPoint DataParser::convertToPolar(QPoint tilt) {
    long altitude = (long)(90 - sqrt((tilt.x() * tilt.x()) + (tilt.y() * tilt.y()))) * 10;
    long azimuth = (long)(1800 + 1800 * atan2(-tilt.x(), -tilt.y()) / 3.14159);//TanHuang arithmetic
    return QPoint(altitude, azimuth);
}


QString formatInt(int value) {
    return QString("%1").arg(value);
}


QString toHex(const QByteArray& data) {
    char hexStr[200] = { 0 };
    ZeroMemory(hexStr, sizeof(hexStr));
    char* pNext = hexStr;
    for (BYTE b : data) {
        wsprintfA(pNext, "%02X ", b);
        pNext += 3;
    }
    return QString(hexStr);
}
