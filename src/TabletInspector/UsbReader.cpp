#include "stdafx.h"
#include "UsbReader.h"


using namespace win32;


UsbReader::UsbReader()  {
}


UsbReader::~UsbReader()  {
    close();
}


bool UsbReader::open(const QString& devicePath) {

    UsbBuffer devBuffer((PBYTE)&_devDesc, sizeof(USB_DEVICE_DESCRIPTOR));
    
    WCHAR szDevicePath[_MAX_PATH] = { 0 };
    ZeroMemory(szDevicePath, _MAX_PATH * sizeof(WCHAR));
    devicePath.toWCharArray(szDevicePath);
    if (!_usb.open(szDevicePath))
        goto error_exit;
    if (!_usb.getDescriptor(USB_DEVICE_DESCRIPTOR_TYPE, 0, 0, devBuffer))
        goto error_exit;
    if (!_usb.queryInterfaceSettings(0, _intfDesc))
        goto error_exit;
    if (!_usb.queryPipe(0, 0, _pipeInfo))
        goto error_exit;

    BYTE buf[128] = { 0 };
    if (!sendControl(201, buf, sizeof(buf)) || !parse_201(buf))
        goto error_exit;
    if (!sendControl(200, buf, sizeof(buf)) || !parse_200(buf))
        goto error_exit;
    if (!sendControl(202, buf, sizeof(buf)) || !parse_202(buf))
        goto error_exit;

    return true;
error_exit:
    close();
    return false;
}


void UsbReader::close() {
    _usb.close();
}


bool UsbReader::sendControl(int nCmd, PBYTE buf, size_t bufSize) {
    WINUSB_SETUP_PACKET wsp;
    wsp.RequestType = 0x80;
    wsp.Request = 0x06;
    wsp.Index = 0x0409;
    wsp.Length = 0x5678;
    wsp.Value = 0x0300 | nCmd;
    memset(buf, 0, bufSize);
    UsbBuffer wub(buf, bufSize);
    bool bRet = _usb.controlTransfer(wsp, wub);
    return bRet;
}


bool UsbReader::parse_201(const BYTE* pBuf) {
    QString value = QString::fromWCharArray((PCWSTR)(pBuf + 2));
    QStringList parts = value.split('_');
    if (parts.size() != 3)
        return false;

    _tabletInfo.oem = parts[0];
    _tabletInfo.model = parts[1];
    _tabletInfo.fixtureDate = parts[2];
    return true;
}


bool UsbReader::parse_200(const BYTE* pBuf) {
    const BYTE* pData = pBuf + 2;
    int cx = (pData[2] << 16) | (pData[1] << 8) | pData[0];
    int cy = (pData[5] << 16) | (pData[4] << 8) | pData[3];
    _tabletInfo.size = QSize(cx, cy);
    _tabletInfo.maxPressure = (pData[7] << 8) | pData[6];
    _tabletInfo.pbtnNum = pData[10];
    _tabletInfo.hbtnNum = pData[11];
    _tabletInfo.sbtnNum = pData[12];
    _tabletInfo.lpi = (pData[9] << 8) | pData[8];
    _tabletInfo.rate = pData[16] * 4;
    _tabletInfo.isMonitor = (pData[14] & 0xa) != 0;
    _tabletInfo.isPassive = (pData[14] & 0xc) != 0;

    return true;
}


bool UsbReader::parse_202(const BYTE* pBuf) {
    size_t maxSize = 64;
    PWSTR pStart = (PWSTR)(pBuf + 2);
    int endIndex = 63;
    for (int i = 0; i < maxSize; i++) {
        WCHAR ch = *(pStart + i);
        if (ch == 0 || ch >= 0x80) {
            endIndex = i;
            break;
        }
    }
    _tabletInfo.company = QString::fromWCharArray(pStart, endIndex);
    return true;
}


bool UsbReader::read(UsbBuffer& ub) {
    return _usb.readPipe(_pipeInfo.PipeId, ub);
}