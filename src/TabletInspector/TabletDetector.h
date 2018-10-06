#pragma once


class TabletDetector : public QObject {
    Q_OBJECT
public:
    TabletDetector(QMainWindow* parent, const GUID& intfClsId);
    virtual ~TabletDetector();

    void detectOnStartup();
    void detectOnDeviceChange(MSG& msg);

    bool isConnected();

signals:
    void tabletConnected(const QString& devicePath);
    void tabletDisconnected(const QString& devicePath);

private:
    GUID        _intfClsId;
    QString     _devicePath;

    void registerNotification(QMainWindow* win);
    PSP_DEVICE_INTERFACE_DETAIL_DATA getInterfaceDetail(HDEVINFO hDevInfo,
        PSP_DEVINFO_DATA pDevInfo, 
        PSP_DEVICE_INTERFACE_DATA pIntfData);
    void releaseInterfaceDetail(PSP_DEVICE_INTERFACE_DETAIL_DATA pData);
};

