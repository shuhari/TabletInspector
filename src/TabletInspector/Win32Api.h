#pragma once


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
            }
            else {
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

};
