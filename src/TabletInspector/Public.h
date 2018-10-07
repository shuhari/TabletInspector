#pragma once


struct TabletInfo {
    QString     devicePath;
    QString     oem;
    QString     model;
    QString     fixtureDate;
    QString     company;
    QSize       size;
    quint32     maxPressure;
    quint16     pBtnNum;
    quint16     hBtnNum;
    quint16     sBtnNum;
    quint32     lpi;
    quint32     rate;
    bool        isMonitor;
    bool        isPassive;

    QString tabletName();
};


extern GUID GUID_DEVINTERFACE_TABLET_WINUSB;


class DataParser {
public:
    enum DataType {
        Unknown = 0,
        PenDown = 1,
        PenUp = 2,
        PenBtnDown = 3,
        TouchBtn = 4,
        Slider = 5,
    };

    DataParser(const QByteArray& data);

    inline DataType dataType() { return _dataType; }

    int touchBtnIndex();
    int sliderId();
    QPoint position();
    int pressure();
    int penBtnIndex();

private:
    const QByteArray& _data;
    DataType _dataType;

    DataType getDataType();
};
