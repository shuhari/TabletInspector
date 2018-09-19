#pragma once


namespace win32 {

    class LastErrorSupport {
    public:
        LastErrorSupport();

        DWORD lastError();
        void recordLastError();
        void clearLastError();
        bool hasLastError();
        bool recordAndReturn(BOOL result);
        bool recordAndReturn(bool result);

    protected:
        void setLastError(DWORD dwError);

    private:
        DWORD _lastError;
    };  // class LastErrorSupport

    interface IMemAllocator {
    public:
        PBYTE alloc(size_t size);
        void release(PBYTE pBuf);
    protected:
        virtual PBYTE allocCore(size_t size) = 0;
        virtual void releaseCore(PBYTE buf) = 0;
    };  // class IMemAllocator

    class CppMemAllocator : public IMemAllocator {
    protected:
        virtual PBYTE allocCore(size_t size) override;
        virtual void releaseCore(PBYTE pBuf) override;
    };  // class CppMemAllocator

    class HeapMemAllocator : public IMemAllocator {
    protected:
        virtual PBYTE allocCore(size_t size) override;
        virtual void releaseCore(PBYTE pBuf) override;
    }; // class HeapMemAllocator

    class MemAllocatorRegistry {
    public:
        static IMemAllocator& cpp();
        static IMemAllocator& heap();
    private:
        static IMemAllocator* _cpp;
        static IMemAllocator* _heap;
    }; // class MemAllocatorRegistry

    template<typename T>
    class HandleSupport {
    public:
        inline HandleSupport(T nullValue) {
            _nullValue = nullValue;
            _handle = nullValue;
        }

        inline virtual ~HandleSupport() {
            close();
        }

        inline T handle() { return _handle; }
        inline bool isOpen() { return _handle != _nullValue; }

        inline virtual bool close() {
            if (_handle != _nullValue) {
                bool bRet = closeCore(_handle);
                if (bRet) {
                    _handle = _nullValue;
                }
                return bRet;
            }
            return true;
        }

    protected:
        T _handle;
        T _nullValue;

    protected:
        virtual bool closeCore(T value) = 0;
    };  // class HandleSupport

    class File : public LastErrorSupport,
        public HandleSupport<HANDLE> {
    public:
        File();
        virtual ~File();

        bool open(PCWSTR pFileName, 
            DWORD dwSesiredAccess, 
            DWORD dwShareMode, 
            PSECURITY_ATTRIBUTES pSecurityAttrs,
            DWORD dwCreationDisposition, 
            DWORD dwFlagsAndAttrs, 
            HANDLE hTemplateFile = NULL);

    protected:
        virtual bool closeCore(HANDLE value) override;
    };
};  // namespace win32