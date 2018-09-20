#include "win32api.h"


using namespace win32;


thread_local DWORD LastError::_lastError = 0;


IAllocator* AllocatorRegistry::_cpp = new CppAllocator();
IAllocator* AllocatorRegistry::_heap = new HeapAllocator();


void LastError::record() {
    _lastError = GetLastError();
}


bool LastError::recordResult(BOOL retValue) {
    if (!retValue) {
        recordLastError();
        return false;
    }
    return true;
}


PVOID IAllocator::alloc(size_t size) {
    assert(size > 0);
    PVOID result = internalAlloc(size);
    assert(result != nullptr);
    ZeroMemory(result, size);
    return result;
}


void IAllocator::release(PVOID buf) {
    if (buf) {
        internalRelease(buf);
    }
}



PVOID IAllocator::copy(PVOID src, size_t size) {
    assert(size > 0);
    assert(!IsBadReadPtr(src, size));

    PVOID result = alloc(size);
    memcpy(result, src, size);
    return result;
}


PVOID CppAllocator::internalAlloc(size_t size) {
    return new BYTE[size];
}


void CppAllocator::internalRelease(PVOID buf) {
    delete [](PBYTE)buf;
}


PVOID HeapAllocator::internalAlloc(size_t size) {
    PVOID result = HeapAlloc(GetProcessHeap(), 0, size);
    if (!result)
        LastError::record();
    return result;
}


void HeapAllocator::internalAlloc(PVOID buf) {
    BOOL bRet = HeapFree(GetProcessHeap(), 0, buf);
    recordResult(bRet);
}


File::~File() {
    close();
}


bool File::internalClose(HANDLE handle) {
    return CloseHandle(handle);
}


bool File::open(PCWSTR pFileName,
    DWORD dwSesiredAccess,
    DWORD dwShareMode,
    PSECURITY_ATTRIBUTES pSecurityAttrs,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttrs,
    HANDLE hTemplateFile/* = NULL*/) {

    close();
    _handle = CreateFile(pFileName, dwSesiredAccess, dwShareMode, pSecurityAttrs,
        dwCreationDisposition, dwFlagsAndAttrs, hTemplateFile);
    return LastError::recordResult(isOpen());
}


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
