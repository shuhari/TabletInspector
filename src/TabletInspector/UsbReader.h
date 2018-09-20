#pragma once


#include "stdafx.h"
#include "DeviceApi.h"
#include "Models.h"


class UsbReader
{
public:
    UsbReader();
    ~UsbReader();

    bool open(const QString& devicePath);
    void close();

    inline TabletInfo& tabletInfo() { return _tabletInfo; }

private:
    TabletInfo                  _tabletInfo;
    win32::WinUsb               _usb;
    USB_DEVICE_DESCRIPTOR       _devDesc;
    USB_INTERFACE_DESCRIPTOR    _intfDesc;
    WINUSB_PIPE_INFORMATION     _pipeInfo;

    bool sendControl(int cmd, PBYTE pBuf, size_t bufSize);
    bool parse_200(const BYTE* buf);
    bool parse_201(const BYTE* buf);
    bool parse_202(const BYTE* buf);
};

