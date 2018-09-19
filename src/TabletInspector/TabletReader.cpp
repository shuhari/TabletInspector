#include "stdafx.h"
#include "TabletReader.h"


using namespace win32;


TabletReader::TabletReader(HWND hwndNotify) 
    : _hwndNotify(hwndNotify),
    _hThread(INVALID_HANDLE_VALUE) {

}


TabletReader::~TabletReader() {

}


bool TabletReader::open(PCWSTR szDevicePath) {
    WinUsbBuffer devBuffer((PBYTE)&_devDescriptor, sizeof(_devDescriptor));
    if (!_usb.open(szDevicePath))
        goto error_exit;

    if (!_usb.getDescriptor(USB_DEVICE_DESCRIPTOR_TYPE, 
        0, 0, devBuffer))
        goto error_exit;

    if (!_usb.queryInterfaceSettings(0, _intfDescriptor))
        goto error_exit;
    if (!_usb.queryPipe(0, 0, _pipeInfo))
        goto error_exit;
    
    BYTE buf[128] = { 0 };
    if (!sendControl(201, buf, sizeof(buf)) ||
        !parse_201(buf))
        goto error_exit;
    if (!sendControl(200, buf, sizeof(buf)) ||
        !parse_200(buf))
        goto error_exit;
    if (!sendControl(202, buf, sizeof(buf)) ||
        !parse_202(buf))
        goto error_exit;

    _hThread = (HANDLE)_beginthreadex(NULL, 0, threadProc, this, 0, 0);
    return true;

error_exit:
    close();
    return false;
}


void TabletReader::close() {
    _usb.close();
}


void TabletReader::waitForExit(int milliseconds) {
    if (_hThread != INVALID_HANDLE_VALUE) {
        DWORD dwResult = WaitForSingleObject(_hThread, milliseconds);
    }
}


bool TabletReader::sendControl(int nCmd, PBYTE buf, size_t bufSize) {
    WINUSB_SETUP_PACKET wsp;
    wsp.RequestType = 0x80;
    wsp.Request = 0x06;
    wsp.Index = 0x0409;
    wsp.Length = 0x5678;
    wsp.Value = 0x0300 | nCmd;
    memset(buf, 0, bufSize);
    WinUsbBuffer wub(buf, bufSize);
    bool bRet = _usb.controlTransfer(wsp, wub);
    return bRet;
}


bool TabletReader::parse_201(const BYTE* pBuf) {
    CString value = (PCWSTR)(pBuf + 2);
    int iStart = 0, iIndex = 0;
    while (true) {
        CString nextPart = value.Tokenize(L"_", iStart);
        if (nextPart.IsEmpty())
            break;
        switch (iIndex) {
        case 0:
            _tabletInfo.oem = nextPart;
            break;
        case 1:
            _tabletInfo.model = nextPart;
            break;
        case 2:
            _tabletInfo.fixtureDate = nextPart;
            break;
        }
        iIndex++;
    }
    return true;
}


bool TabletReader::parse_200(const BYTE* pBuf) {
    const BYTE* pData = pBuf + 2;
    _tabletInfo.size.cx = (pData[2] << 16) | (pData[1] << 8) | pData[0];
    _tabletInfo.size.cy = (pData[5] << 16) | (pData[4] << 8) | pData[3];
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


bool TabletReader::parse_202(const BYTE* pBuf) {
    PWSTR pStart = (PWSTR)(pBuf + 2);
    int iIndex = 63;
    for (int i = 0; i < 63; i++) {
        WCHAR ch = *(pStart + i);
        if (ch == 0 || ch >= 0x80) {
            iIndex = i;
            break;
        }
    }
    WCHAR szBuf[64];
    ZeroMemory(szBuf, sizeof(szBuf));
    memcpy(szBuf, pStart, iIndex * sizeof(WCHAR));
    _tabletInfo.company = szBuf;
    return true;
}


UINT __stdcall TabletReader::threadProc(PVOID pParam) {
    ATLASSERT(pParam != nullptr);
    TabletReader* pReader = (TabletReader*)pParam;

    while (true) {
        Sleep(20);
    }

    delete pReader;
    return 0;
}