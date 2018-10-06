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
