#pragma once


#include "precompiled.h"


namespace win32 {

    class LastError {
    public:
        inline static DWORD current() { return _lastError; }
        inline static bool hasError() { return _lastError != ERROR_SUCCESS; }
        inline static void clear() { _lastError = ERROR_SUCCESS; }

        static void record();
        static bool recordResult(BOOL retValue);

    private:
        static thread_local DWORD _lastError;
    };  // class LastErrorSupport

    struct IAllocator {
    public:
        virtual ~IAllocator() = default;

        PVOID alloc(size_t size);
        PVOID copy(PVOID src, size_t size);
        void release(PVOID buf);

    protected:
        virtual PVOID internalAlloc(size_t size) = 0;
        virtual void internalRelease(PVOID buf) = 0;
    };  // struct IAllocator

    class CppAllocator : public IAllocator {
    public:
        virtual ~CppAllocator() override = default;

    protected:
        virtual PVOID internalAlloc(size_t size) override;
        virtual void internalRelease(PVOID buf) override;
    };  // class CppAllocator

    class HeapAllocator : public IAllocator {
    public:
        virtual ~HeapAllocator() override = default;

    protected:
        virtual PVOID internalAlloc(size_t size) override;
        virtual void internalRelease(PVOID buf) override;
    };  // class HeapAllocator

    class AllocatorRegistry {
    public:
        inline static IAllocator& cpp() { return *_cpp; }
        inline static IAllocator& heap() { return *_heap; }

    private:
        static IAllocator* _cpp;
        static IAllocator* _heap;
    };  // class AllocatorRegistry

    template<typename T, T nullValue>
    class HandleBase {
    public:
        inline HandleBase() : _handle(nullValue) {}
        inline virtual ~HandleBase() = default;

        inline T handle() { return _handle; }
        inline bool isOpen() { return _handle != nullValue; }

        inline bool close() {
            if (_handle == nullValue)
                return true;
            bool bRet = internalClose(_handle);
            if (bRet) {
                _handle = nullValue;
                return true;
            } else {
                LastError::record();
                return false;
            }
        }

    protected:
        T _handle;

        virtual bool internalClose(T handle) = 0;
    };  // class HandleBase

    class File : public HandleBase<HANDLE, INVALID_HANDLE_VALUE> {
    public:
        File();
        virtual ~File() override;

        bool open(PCWSTR pFileName,
                    DWORD dwSesiredAccess,
                    DWORD dwShareMode,
                    PSECURITY_ATTRIBUTES pSecurityAttrs,
                    DWORD dwCreationDisposition,
                    DWORD dwFlagsAndAttrs,
                    HANDLE hTemplateFile = NULL);

    protected:
        virtual bool internalClose(HANDLE handle) override;
    };  // class File

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
};
