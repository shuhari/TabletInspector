#include "stdafx.h"
#include "UsbDetector.h"
#include "DeviceApi.h"
#include "Public.h"


using namespace win32;


UsbDetector::UsbDetector(QMainWindow* parent, const GUID& intfClsId) :
    QObject(parent),
    _intfClsId(intfClsId), 
    _connectedDevicePath("") {

    registerNotification(parent);
}


UsbDetector::~UsbDetector() {
}


void UsbDetector::registerNotification(QMainWindow* win) {
    DEV_BROADCAST_DEVICEINTERFACE dbd;
    ZeroMemory(&dbd, sizeof(dbd));
    dbd.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    dbd.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    dbd.dbcc_classguid = _intfClsId;
    RegisterDeviceNotification((HANDLE)win->winId(), 
        &dbd, 
        DEVICE_NOTIFY_WINDOW_HANDLE);
}


bool UsbDetector::isConnected() {
    return !_connectedDevicePath.isEmpty();
}


void UsbDetector::detectOnStartUp() {

    QStringList devicePaths;
    DeviceInfoSet dis;
    if (!dis.open(&_intfClsId, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE))
        return;

    DWORD dwDevIndex = 0;
    SP_DEVINFO_DATA devInfo;
    while (true) {
        BOOL bRet = dis.enumInfo(dwDevIndex, devInfo);
        if (!bRet)
            break;
        SP_DEVICE_INTERFACE_DATA intfData;
        DWORD dwIntfIndex = 0;
        while (true) {
            if (dis.enumInterfaces(&devInfo, &_intfClsId, dwIntfIndex, intfData)) {
                auto pDetail = dis.getInterfaceDetail(&intfData, &devInfo);
                if (pDetail) {
                    devicePaths.append(QString::fromWCharArray(pDetail->DevicePath));
                    dis.releaseInfoDetail(pDetail);
                }
                dwIntfIndex++;
            }
            else
                break;
        }
        dwDevIndex++;
    }

    if (devicePaths.size() == 1) {
        _connectedDevicePath = devicePaths[0];
        emit deviceConnected(_connectedDevicePath);
    }
}


void UsbDetector::detectOnDeviceChangeEvent(MSG& msg) {
    if (!msg.lParam) 
        return;

    auto pdbh = (PDEV_BROADCAST_HDR)msg.lParam;
    if (pdbh->dbch_devicetype != DBT_DEVTYP_DEVICEINTERFACE)
        return;
    auto pIntf = (PDEV_BROADCAST_DEVICEINTERFACE)pdbh;
    if (!pIntf->dbcc_name)
        return;
    QString deviceName = QString::fromWCharArray(pIntf->dbcc_name);

    switch (msg.wParam) {
    case DBT_DEVICEARRIVAL:
        if (!isConnected()) {
            _connectedDevicePath = deviceName;
            emit deviceConnected(_connectedDevicePath);
        }
        break;

    case DBT_DEVICEREMOVECOMPLETE:
        if (isConnected() && 
            _connectedDevicePath.toLower() == deviceName.toLower()) {
            _connectedDevicePath = "";
            emit deviceDisconnected(deviceName);
        }
        break;
    }
}