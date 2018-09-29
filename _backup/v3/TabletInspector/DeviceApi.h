#pragma once


#include "Win32Api.h"


namespace win32 {

    class DeviceInfoSet : public HandleBase<HDEVINFO, INVALID_HANDLE_VALUE> {
    public:
        DeviceInfoSet();
        virtual ~DeviceInfoSet() override;

        inline static IAllocator& allocator() { return AllocatorRegistry::cpp(); }

        bool open(const GUID* clsId, PCWSTR pEnum, HWND hwndParent, DWORD dwFlags);
        bool enumInfo(DWORD memberIndex, SP_DEVINFO_DATA& devInfo);
        bool enumInterfaces(PSP_DEVINFO_DATA pDevInfo, const GUID* intfClsId, DWORD memberIndex, SP_DEVICE_INTERFACE_DATA& intfData);
        PSP_DEVICE_INTERFACE_DETAIL_DATA getInterfaceDetail(PSP_DEVICE_INTERFACE_DATA pIntfData,
            PSP_DEVINFO_DATA pDevInfo);
        void releaseInfoDetail(PSP_DEVICE_INTERFACE_DETAIL_DATA pDetail);

    protected:
        virtual bool internalClose(HDEVINFO handle) override;

    }; // class DeviceInfoSet

    struct UsbBuffer {
    public:
        UsbBuffer(PBYTE pBuf, size_t size, LPOVERLAPPED pOverlapped = NULL);

        PBYTE           buffer;
        ULONG           bufferSize;
        ULONG           transferred;
        LPOVERLAPPED    pOverlapped;
    };  // struct UsbBuffer

    class WinUsb : public HandleBase<WINUSB_INTERFACE_HANDLE, NULL> {
    public:
        WinUsb();
        virtual ~WinUsb() override;

        bool open(PCWSTR szDevicePath);
        bool getDescriptor(BYTE descriptorType, BYTE index, USHORT languageId, UsbBuffer& ub);
        bool queryInterfaceSettings(BYTE altIntfNum, USB_INTERFACE_DESCRIPTOR& descriptor);
        bool queryPipe(BYTE altIntfNum, BYTE pipeId, WINUSB_PIPE_INFORMATION& pipeInfo);
        bool controlTransfer(WINUSB_SETUP_PACKET& packet, UsbBuffer& ub);
        bool readPipe(BYTE pipeId, UsbBuffer& ub);

    protected:
        virtual bool internalClose(WINUSB_INTERFACE_HANDLE handle) override;

    private:
        File _file;
    };  // class WinUsb

};  // namespace win32
