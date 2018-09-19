#pragma once

#include "DeviceApi.h"
#include "TabletInfo.h"


class TabletReader {
public:
    TabletReader(HWND hwndNotify);
    ~TabletReader();

    bool open(PCWSTR szDevicePath);
    void close();
    void waitForExit(int milliseconds);

    inline TabletInfo& tabletInfo() { return _tabletInfo; }

private:
    HWND                        _hwndNotify;
    HANDLE                      _hThread;
    win32::WinUsb               _usb;
    USB_DEVICE_DESCRIPTOR       _devDescriptor;
    USB_INTERFACE_DESCRIPTOR    _intfDescriptor;
    WINUSB_PIPE_INFORMATION     _pipeInfo;
    TabletInfo                  _tabletInfo;

    static UINT __stdcall threadProc(PVOID pParam);
    bool sendControl(int nCmd, PBYTE buf, size_t bufSize);
    bool parse_201(const BYTE* pBuf);
    bool parse_200(const BYTE* pBuf);
    bool parse_202(const BYTE* pBuf);
};