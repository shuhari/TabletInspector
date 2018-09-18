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
    if (!_usb.open(szDevicePath))
        goto error_exit;

    WinUsbBuffer devBuffer((PBYTE)&_devDescriptor, sizeof(_devDescriptor));
    if (!_usb.getDescriptor(USB_DEVICE_DESCRIPTOR_TYPE, 
        0, 0, devBuffer))
        goto error_exit;

    if (!_usb.queryInterfaceSettings(0, _intfDescriptor))
        goto error_exit;
    if (!_usb.queryPipe(0, 0, _pipeInfo))
        goto error_exit;
    
    BYTE buf[64] = { 0 };
    if (!sendControl(201, buf, sizeof(buf)) ||
        !parse_201(buf))
        goto error_exit;
    if (!sendControl(200, buf, sizeof(buf)) ||
        !parse_200(buf))
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