#include "stdafx.h"
#include "Win32Api.h"


using namespace win32;


LastErrorSupport::LastErrorSupport() 
    : _lastError(ERROR_SUCCESS) {
}


DWORD LastErrorSupport::lastError() {
    return _lastError;
}


void LastErrorSupport::recordLastError() {
    _lastError = GetLastError();
}


bool LastErrorSupport::hasLastError() {
    return _lastError != ERROR_SUCCESS;
}


void LastErrorSupport::clearLastError() {
    _lastError = ERROR_SUCCESS;
}


bool LastErrorSupport::recordAndReturn(BOOL result) {
    if (!result) {
        recordLastError();
        return false;
    }
    return true;
}


bool LastErrorSupport::recordAndReturn(bool result) {
    if (!result) {
        recordLastError();
        return false;
    }
    return true;
}


IMemAllocator* MemAllocatorRegistry::_cpp = new CppMemAllocator();
IMemAllocator* MemAllocatorRegistry::_heap = new HeapMemAllocator();

PBYTE IMemAllocator::alloc(size_t size) {
    ATLASSERT(size > 0);
    auto result = allocCore(size);
    ATLASSERT(result != nullptr);
    ZeroMemory(result, size);
    return result;
}


void IMemAllocator::release(PBYTE pBuf) {
    if (pBuf) {
        releaseCore(pBuf);
    }
}


PBYTE CppMemAllocator::allocCore(size_t size) {
    return new BYTE[size];
}


void CppMemAllocator::releaseCore(PBYTE pBuf) {
    delete[]pBuf;
}


PBYTE HeapMemAllocator::allocCore(size_t size) {
    return (PBYTE)HeapAlloc(GetProcessHeap(), 0, size);
}


void HeapMemAllocator::releaseCore(PBYTE pBuf) {
    BOOL bRet = HeapFree(GetProcessHeap(), 0, pBuf);
    ATLASSERT(bRet);
}


IMemAllocator& MemAllocatorRegistry::cpp() {
    return *_cpp;
}


IMemAllocator& MemAllocatorRegistry::heap() {
    return *_heap;
}


File::File()
    : HandleSupport(INVALID_HANDLE_VALUE) {
}


File::~File() {
}


bool File::closeCore(HANDLE value) {
    BOOL bRet = CloseHandle(value);
    return recordAndReturn(bRet);
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
    if (!recordAndReturn(isOpen()))
        return false;

    return true;

}