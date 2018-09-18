#include "stdafx.h"
#include "DeviceApi.h"


using namespace win32;


DeviceInfoSet::DeviceInfoSet()
    : HandleSupport<HDEVINFO>(INVALID_HANDLE_VALUE) {

}


DeviceInfoSet::~DeviceInfoSet() {
    close();
}


bool DeviceInfoSet::open(const GUID* clsId, PCWSTR pEnum, HWND hwndParent, DWORD dwFlags) {
    close();
    _handle = SetupDiGetClassDevs(clsId, pEnum, hwndParent, dwFlags);
    if (!recordAndReturn(isOpen()))
        return false;
    return true;
}


bool DeviceInfoSet::closeCore(HDEVINFO handle) {
    BOOL bRet = SetupDiDestroyDeviceInfoList(handle);
    return recordAndReturn(bRet);
}

bool DeviceInfoSet::enumInfo(DWORD memberIndex, SP_DEVINFO_DATA& devInfo) {
    ZeroMemory(&devInfo, sizeof(devInfo));
    devInfo.cbSize = sizeof(devInfo);
    BOOL bRet = SetupDiEnumDeviceInfo(handle(), memberIndex, &devInfo);
    return recordAndReturn(bRet);
}


bool DeviceInfoSet::enumInterfaces(PSP_DEVINFO_DATA pDevInfo, const GUID* intfClsId, DWORD memberIndex, SP_DEVICE_INTERFACE_DATA& intfData) {
    ZeroMemory(&intfData, sizeof(intfData));
    intfData.cbSize = sizeof(intfData);
    BOOL bRet = SetupDiEnumDeviceInterfaces(handle(), pDevInfo, intfClsId, memberIndex, &intfData);
    return recordAndReturn(bRet);
}


PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceInfoSet::getInterfaceDetail(PSP_DEVICE_INTERFACE_DATA pIntfData,
    PSP_DEVINFO_DATA pDevInfo) {

    DWORD dwSize = 0;
    IMemAllocator& allocator = MemAllocatorRegistry::cpp();
    BOOL bRet = SetupDiGetDeviceInterfaceDetail(handle(), pIntfData, NULL, 0, &dwSize, pDevInfo);
    if (!bRet && GetLastError() == ERROR_INSUFFICIENT_BUFFER && dwSize > 0) {
        auto pDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)allocator.alloc(dwSize);
        pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
        bRet = SetupDiGetDeviceInterfaceDetail(handle(), pIntfData, pDetail, dwSize, &dwSize, pDevInfo);
        if (bRet) {
            return pDetail;
        }
        else {
            recordLastError();
            releaseInfoDetail(pDetail);
            return nullptr;
        }
    }

    recordLastError();
    return nullptr;
}


void DeviceInfoSet::releaseInfoDetail(PSP_DEVICE_INTERFACE_DETAIL_DATA pDetail) {
    IMemAllocator& allocator = MemAllocatorRegistry::cpp();
    allocator.release((PBYTE)pDetail);
}


WinUsbBuffer::WinUsbBuffer(PBYTE pBuf, size_t bufSize, LPOVERLAPPED pOverlapped/*=NULL*/) {
    this->buffer = pBuf;
    this->bufferSize = (ULONG)bufSize;
    this->transferred = 0;
    this->pOverlapped = pOverlapped;
}


WinUsb::WinUsb()
    : HandleSupport<WINUSB_INTERFACE_HANDLE>(NULL) {
}


WinUsb::~WinUsb() {
}


bool WinUsb::closeCore(WINUSB_INTERFACE_HANDLE handle) {
    bool bCloseUsbOk = false, bCloseFileOk = false;
    if (isOpen()) {
        BOOL bRet = WinUsb_Free(handle);
        bCloseUsbOk = recordAndReturn(bRet);
    }
    if (_file.isOpen()) {
        bCloseFileOk = _file.close();
    }
    return bCloseUsbOk && bCloseFileOk;
}


bool WinUsb::open(PCWSTR szDevicePath) {
    ATLASSERT(szDevicePath != nullptr);
    close();

    if (!_file.open(szDevicePath,
        GENERIC_WRITE | GENERIC_READ,
        FILE_SHARE_WRITE | FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED, NULL))
        goto error_exit;
    BOOL bRet = WinUsb_Initialize(_file.handle(), &_handle);
    if (!recordAndReturn(bRet))
        return false;
    return true;

error_exit:
    close();
    return false;
}


bool WinUsb::getDescriptor(BYTE descriptorType, BYTE index, USHORT languageId,
    WinUsbBuffer& wub) {
    BOOL bRet = WinUsb_GetDescriptor(_handle,
        descriptorType,
        index, 
        languageId,
        wub.buffer,
        wub.bufferSize,
        &wub.transferred);
    return recordAndReturn(bRet);
}


bool WinUsb::queryInterfaceSettings(BYTE altIntfNum, USB_INTERFACE_DESCRIPTOR& descriptor) {
    BOOL bRet = WinUsb_QueryInterfaceSettings(_handle, altIntfNum, &descriptor);
    return recordAndReturn(bRet);
}


bool WinUsb::queryPipe(BYTE altIntfNum, BYTE pipeId, WINUSB_PIPE_INFORMATION& pipeInfo) {
    BOOL bRet = WinUsb_QueryPipe(_handle, altIntfNum, pipeId, &pipeInfo);
    return recordAndReturn(bRet);
}


bool WinUsb::controlTransfer(WINUSB_SETUP_PACKET& packet, WinUsbBuffer& wub) {
    BOOL bRet = WinUsb_ControlTransfer(_handle, 
        packet,
        wub.buffer, 
        wub.bufferSize, 
        &wub.transferred, 
        wub.pOverlapped);
    return recordAndReturn(bRet);
}


bool WinUsb::readPipe(UCHAR pipeId, WinUsbBuffer& wub) {
    BOOL bRet = WinUsb_ReadPipe(_handle, 
        pipeId, 
        wub.buffer, 
        wub.bufferSize, 
        &wub.transferred, 
        wub.pOverlapped);
    return recordAndReturn(bRet);
}