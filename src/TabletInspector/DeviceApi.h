#pragma once


#include "Win32Api.h"


namespace win32 {
    
    class DeviceInfoSet : public LastErrorSupport,
        public HandleSupport<HDEVINFO> {
    public:
        DeviceInfoSet();
        virtual ~DeviceInfoSet();

        bool open(const GUID* clsId, PCWSTR pEnum, HWND hwndParent, DWORD dwFlags);

        bool enumInfo(DWORD memberIndex, SP_DEVINFO_DATA& devInfo);
        bool enumInterfaces(PSP_DEVINFO_DATA pDevInfo, const GUID* intfClsId, DWORD memberIndex, SP_DEVICE_INTERFACE_DATA& intfData);
        PSP_DEVICE_INTERFACE_DETAIL_DATA getInterfaceDetail(PSP_DEVICE_INTERFACE_DATA pIntfData,
            PSP_DEVINFO_DATA pDevInfo);
        void releaseInfoDetail(PSP_DEVICE_INTERFACE_DETAIL_DATA pDetail);

    protected:
        virtual bool closeCore(HDEVINFO handle) override;
    };  // DeviceInfoSet

    struct WinUsbBuffer {
    public:
        WinUsbBuffer(PBYTE pBuf, size_t bufSize, LPOVERLAPPED pOverlapped = NULL);
        PBYTE        buffer;
        ULONG        bufferSize;
        ULONG        transferred;
        LPOVERLAPPED pOverlapped;
    };  // struct WinUsbBuffer

    class WinUsb : public LastErrorSupport,
        public HandleSupport<WINUSB_INTERFACE_HANDLE> {
    public:
        WinUsb();
        virtual ~WinUsb();

        bool open(PCWSTR szDevicePath);
        bool getDescriptor(BYTE descriptorType, BYTE index, USHORT languageId, WinUsbBuffer& wub);
        bool queryInterfaceSettings(BYTE altIntfNum, USB_INTERFACE_DESCRIPTOR& descriptor);
        bool queryPipe(BYTE altIntfNum, BYTE pipeId, WINUSB_PIPE_INFORMATION& pipeInfo);
        bool controlTransfer(WINUSB_SETUP_PACKET& packet, WinUsbBuffer& wub);
        bool readPipe(BYTE pipeId, WinUsbBuffer& wub);

    protected:
        virtual bool closeCore(WINUSB_INTERFACE_HANDLE handle) override;

    private:
        File _file;
    };  // class WinUsb

};  // namespace win32
