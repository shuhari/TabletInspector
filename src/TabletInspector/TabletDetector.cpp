#include "stdafx.h"
#include "TabletDetector.h"


TabletDetector::TabletDetector(QMainWindow* parent, const GUID& intfClsId) :
    QObject(parent),
    _intfClsId(intfClsId),
    _devicePath("") {

    registerNotification(parent);
}


TabletDetector::~TabletDetector() {
}


void TabletDetector::registerNotification(QMainWindow* win) {
    DEV_BROADCAST_DEVICEINTERFACE dbd;
    ZeroMemory(&dbd, sizeof(dbd));
    dbd.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    dbd.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    dbd.dbcc_classguid = _intfClsId;
    RegisterDeviceNotification((HANDLE)win->winId(),
        &dbd,
        DEVICE_NOTIFY_WINDOW_HANDLE);
}


bool TabletDetector::isConnected() {
    return !_devicePath.isEmpty();
}


void TabletDetector::detectOnStartup() {
    QStringList detectedPaths;

    HANDLE hDevInfo = SetupDiGetClassDevs(&_intfClsId, NULL, NULL,
        DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (hDevInfo == INVALID_HANDLE_VALUE)
        return;

    SP_DEVINFO_DATA devData;
    DWORD dwDevIndex = 0;
    while (true) {
        ZeroMemory(&devData, sizeof(SP_DEVINFO_DATA));
        devData.cbSize = sizeof(SP_DEVINFO_DATA);
        BOOL bRet = SetupDiEnumDeviceInfo(hDevInfo, dwDevIndex, &devData);
        if (!bRet)
            break;
        SP_DEVICE_INTERFACE_DATA intfData;
        DWORD dwIntfIndex = 0;
        while (true) {
            ZeroMemory(&intfData, sizeof(SP_DEVICE_INTERFACE_DATA));
            intfData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
            BOOL bRet = SetupDiEnumDeviceInterfaces(hDevInfo, &devData, &_intfClsId, dwIntfIndex, &intfData);
            if (bRet) {
                auto pDetail = getInterfaceDetail(hDevInfo, &devData, &intfData);
                if (pDetail) {
                    detectedPaths << QString::fromWCharArray(pDetail->DevicePath);
                    releaseInterfaceDetail(pDetail);
                }
                dwIntfIndex++;
                continue;
            }
            else {
                break;
            }
        }
        dwDevIndex++;
    }

    if (detectedPaths.size() == 1) {
        _devicePath = detectedPaths[0];
        emit tabletConnected(_devicePath);
    }
}


void TabletDetector::detectOnDeviceChange(MSG& msg) {
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
            _devicePath = deviceName;
            emit tabletConnected(_devicePath);
        }
        break;

    case DBT_DEVICEREMOVECOMPLETE:
        if (isConnected() &&
            _devicePath.toLower() == deviceName.toLower()) {
            emit tabletDisconnected(_devicePath);
            _devicePath = "";
        }
        break;
    }
}


PSP_DEVICE_INTERFACE_DETAIL_DATA TabletDetector::getInterfaceDetail(HDEVINFO hDevInfo, 
    PSP_DEVINFO_DATA pDevInfo, 
    PSP_DEVICE_INTERFACE_DATA pIntfData) {

    DWORD dwSize = 0;
    BOOL bRet = SetupDiGetDeviceInterfaceDetail(hDevInfo, pIntfData, NULL, 0, &dwSize, pDevInfo);
    if (!bRet && GetLastError() == ERROR_INSUFFICIENT_BUFFER && dwSize > 0) {
        auto pDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)new BYTE[dwSize];
        pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
        bRet = SetupDiGetDeviceInterfaceDetail(hDevInfo, pIntfData, pDetail, dwSize, &dwSize, pDevInfo);
        if (!bRet) {
            releaseInterfaceDetail(pDetail);
            return nullptr;
        }
        return pDetail;
    }

    return nullptr;
}


void TabletDetector::releaseInterfaceDetail(PSP_DEVICE_INTERFACE_DETAIL_DATA pData) {
    if (pData) {
        delete[]((PBYTE)pData);
    }
}

