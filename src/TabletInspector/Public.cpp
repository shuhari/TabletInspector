#include "stdafx.h"
#include "Public.h"


// {62F12D4C-3431-4EFD-8DD7-8E9AAB18D30C}
GUID GUID_DEVINTERFACE_TABLET_WINUSB =
    { 0x62F12D4C, 0x3431, 0x4EFD, {0x8D, 0xD7, 0x8E, 0x9A, 0xAB, 0x18, 0xD3, 0x0C} };


QString TabletInfo::tabletName() {
    return QString("%1 %2").arg(oem).arg(model);
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