#include "stdafx.h"
#include "Win32Api.h"


using namespace win32;


thread_local DWORD LastError::_lastError = 0;

IAllocator* AllocatorRegistry::_cpp = new CppAllocator();
IAllocator* AllocatorRegistry::_heap = new HeapAllocator();


void LastError::record() {
    _lastError = GetLastError();
}


bool LastError::recordResult(BOOL retValue) {
    if (!retValue) {
        record();
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
    delete[](PBYTE)buf;
}


PVOID HeapAllocator::internalAlloc(size_t size) {
    PVOID result = HeapAlloc(GetProcessHeap(), 0, size);
    if (!result)
        LastError::record();
    return result;
}


void HeapAllocator::internalRelease(PVOID buf) {
    BOOL bRet = HeapFree(GetProcessHeap(), 0, buf);
    LastError::recordResult(bRet);
}


File::File() {
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


