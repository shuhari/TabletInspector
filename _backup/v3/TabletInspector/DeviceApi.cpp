#include "stdafx.h"
#include "DeviceApi.h"


using namespace win32;


DeviceInfoSet::DeviceInfoSet() {

}


DeviceInfoSet::~DeviceInfoSet() {
    close();
}


bool DeviceInfoSet::open(const GUID* clsId, PCWSTR pEnum, HWND hwndParent, DWORD dwFlags) {
    close();
    _handle = SetupDiGetClassDevs(clsId, pEnum, hwndParent, dwFlags);
    return LastError::recordResult(isOpen());
}


bool DeviceInfoSet::internalClose(HDEVINFO handle) {
    return SetupDiDestroyDeviceInfoList(handle);
}


bool DeviceInfoSet::enumInfo(DWORD memberIndex, SP_DEVINFO_DATA& devInfo) {
    ZeroMemory(&devInfo, sizeof(devInfo));
    devInfo.cbSize = sizeof(devInfo);
    BOOL bRet = SetupDiEnumDeviceInfo(handle(), memberIndex, &devInfo);
    return LastError::recordResult(bRet);
}


bool DeviceInfoSet::enumInterfaces(PSP_DEVINFO_DATA pDevInfo, const GUID* intfClsId, DWORD memberIndex, SP_DEVICE_INTERFACE_DATA& intfData) {
    ZeroMemory(&intfData, sizeof(intfData));
    intfData.cbSize = sizeof(intfData);
    BOOL bRet = SetupDiEnumDeviceInterfaces(handle(), pDevInfo, intfClsId, memberIndex, &intfData);
    return LastError::recordResult(bRet);
}


PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceInfoSet::getInterfaceDetail(PSP_DEVICE_INTERFACE_DATA pIntfData,
    PSP_DEVINFO_DATA pDevInfo) {

    DWORD dwSize = 0;
    BOOL bRet = SetupDiGetDeviceInterfaceDetail(handle(), pIntfData, NULL, 0, &dwSize, pDevInfo);
    LastError::recordResult(bRet);
    if (!bRet && LastError::current() == ERROR_INSUFFICIENT_BUFFER && dwSize > 0) {
        auto pDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)allocator().alloc(dwSize);
        pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
        bRet = SetupDiGetDeviceInterfaceDetail(handle(), pIntfData, pDetail, dwSize, &dwSize, pDevInfo);
        if (!LastError::recordResult(bRet)) {
            releaseInfoDetail(pDetail);
            return nullptr;
        }
        return pDetail;
    }

    return nullptr;
}


void DeviceInfoSet::releaseInfoDetail(PSP_DEVICE_INTERFACE_DETAIL_DATA pDetail) {
    allocator().release((PBYTE)pDetail);
}


UsbBuffer::UsbBuffer(PBYTE pBuf, size_t size, LPOVERLAPPED pOverlapped/*=NULL*/) {
    this->buffer = pBuf;
    this->bufferSize = size;
    this->transferred = 0;
    this->pOverlapped = pOverlapped;
}


WinUsb::WinUsb() {

}


WinUsb::~WinUsb() {
    close();
}


bool WinUsb::internalClose(WINUSB_INTERFACE_HANDLE handle) {
    bool bCloseUsbOk = false, bCloseFileOk = false;
    if (isOpen()) {
        BOOL bRet = WinUsb_Free(handle);
        bCloseUsbOk = LastError::recordResult(bRet);
    }
    if (_file.isOpen()) {
        bCloseFileOk = _file.close();
    }
    return bCloseUsbOk && bCloseFileOk;
}


bool WinUsb::open(PCWSTR szDevicePath) {
    assert(szDevicePath != nullptr);
    close();

    if (!_file.open(szDevicePath,
        GENERIC_WRITE | GENERIC_READ,
        FILE_SHARE_WRITE | FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED, NULL))
        goto error_exit;
    BOOL bRet = WinUsb_Initialize(_file.handle(), &_handle);
    if (!LastError::recordResult(bRet))
        return false;
    return true;

error_exit:
    close();
    return false;
}


bool WinUsb::getDescriptor(BYTE descriptorType, BYTE index, USHORT languageId,
    UsbBuffer& ub) {
    BOOL bRet = WinUsb_GetDescriptor(_handle,
        descriptorType,
        index,
        languageId,
        ub.buffer,
        ub.bufferSize,
        &ub.transferred);
    return LastError::recordResult(bRet);
}

bool WinUsb::queryInterfaceSettings(BYTE altIntfNum, USB_INTERFACE_DESCRIPTOR& descriptor) {
    BOOL bRet = WinUsb_QueryInterfaceSettings(_handle, altIntfNum, &descriptor);
    return LastError::recordResult(bRet);
}


bool WinUsb::queryPipe(BYTE altIntfNum, BYTE pipeId, WINUSB_PIPE_INFORMATION& pipeInfo) {
    BOOL bRet = WinUsb_QueryPipe(_handle, altIntfNum, pipeId, &pipeInfo);
    return LastError::recordResult(bRet);
}


bool WinUsb::controlTransfer(WINUSB_SETUP_PACKET& packet, UsbBuffer& ub) {
    BOOL bRet = WinUsb_ControlTransfer(_handle,
        packet,
        ub.buffer,
        ub.bufferSize,
        &ub.transferred,
        ub.pOverlapped);
    return LastError::recordResult(bRet);
}


bool WinUsb::readPipe(UCHAR pipeId, UsbBuffer& ub) {
    BOOL bRet = WinUsb_ReadPipe(_handle,
        pipeId,
        ub.buffer,
        ub.bufferSize,
        &ub.transferred,
        ub.pOverlapped);
    return LastError::recordResult(bRet);
}
