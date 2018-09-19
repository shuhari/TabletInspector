#pragma once


#include "Win32Api.h"
#include "Public.h"


class ITabletMessage {
public:
    ITabletMessage(UINT msgType);
    virtual ~ITabletMessage() = default;

    inline UINT msgType() { return _msgType; }
    bool post(HWND hwnd, WPARAM wParam = 0);

protected:
    static win32::IMemAllocator& allocator();

private:
    UINT _msgType;
};


class PacketDataMessage : public ITabletMessage {
public:
    PacketDataMessage(PBYTE pBuf, int nSize);
    virtual ~PacketDataMessage();

    inline BYTE* buffer() { return _pBuf; }
    inline int bufSize() { return _nBufSize; }

    bool isValid();
    BYTE packetType();
    CPoint point();
    UINT pressure();
    CPoint rotatePoint();
    UINT hButton();
    UINT slider();
    UINT touchId();

private:
    BYTE*   _pBuf;
    int     _nBufSize;
};