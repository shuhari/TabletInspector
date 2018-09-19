#include "stdafx.h"
#include "Messages.h"


using namespace win32;


ITabletMessage::ITabletMessage(UINT msgType) {
    _msgType = msgType;
}


IMemAllocator& ITabletMessage::allocator() {
    return MemAllocatorRegistry::cpp();
}


bool ITabletMessage::post(HWND hwnd, WPARAM wParam/*=0*/) {
    BOOL bRet = PostMessage(hwnd, WM_TABLET, wParam, (LPARAM)this);
    return bRet ? true : false;
}


PacketDataMessage::PacketDataMessage(PBYTE pBuf, int nSize)
    : ITabletMessage(TABLET_MSG_PACKET_DATA) {
    ATLASSERT(pBuf != nullptr);
    _pBuf = allocator().alloc(nSize);
    memcpy(_pBuf, pBuf, nSize);
    _nBufSize = nSize;
}

PacketDataMessage::~PacketDataMessage() {
    allocator().release(_pBuf);
    _pBuf = nullptr;
    _nBufSize = 0;
}


bool PacketDataMessage::isValid() {
    return _pBuf[0] == 0x08;
}

BYTE PacketDataMessage::packetType() {
    return _pBuf[1];
}


CPoint PacketDataMessage::point() {
    int x = (_pBuf[8] << 16) | (_pBuf[3] << 8) | _pBuf[2];
    int y = (_pBuf[9] << 16) | (_pBuf[5] << 8) | _pBuf[4];
    return CPoint(x, y);
}


UINT PacketDataMessage::pressure() {
    return (_pBuf[7] << 8) | _pBuf[6];
}

CPoint PacketDataMessage::rotatePoint() {
    int ax = _pBuf[10];
    int ay = _pBuf[11];
    return CPoint(ax, ay);
}

UINT PacketDataMessage::hButton() {
    return (_pBuf[5] << 8) | _pBuf[4];
}


UINT PacketDataMessage::slider() {
    return _pBuf[5];
}

UINT PacketDataMessage::touchId() {
    return (_pBuf[5] << 8) | _pBuf[4];
}