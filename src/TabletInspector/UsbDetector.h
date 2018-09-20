#pragma once

#include "stdafx.h"


class UsbDetector : public QObject
{
    Q_OBJECT
public:
    UsbDetector(QMainWindow* parent, const GUID& intfClsId);
    ~UsbDetector();

    void detectOnStartUp();
    void detectOnDeviceChangeEvent(MSG& msg);
    bool isConnected();

signals:
    void deviceConnected(const QString& devicePath);
    void deviceDisconnected(const QString& devicePath);

private:
    GUID    _intfClsId;
    QString _connectedDevicePath;

    void    registerNotification(QMainWindow* win);
};

