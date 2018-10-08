#include "stdafx.h"
#include "TabletReader.h"


TabletReaderThread::TabletReaderThread(QObject* parent)
    : QThread(parent) {
}


void TabletReaderThread::run() {
    auto reader = qobject_cast<TabletReader*>(parent());

    while (!isInterruptionRequested()) {
        QByteArray buffer(128, 0);
        ULONG ulReaded = 0;
        if (reader->read(buffer, &ulReaded) && ulReaded > 0) {
            buffer.resize(ulReaded);
            emit reader->tabletReadData(buffer);
        } else {
            DWORD dwError = GetLastError();
            if (dwError != 0)
                emit reader->tabletReadError(dwError);
        }
    }
    reader->close();
}


TabletReader::TabletReader(QObject* parent) :
    QObject(parent),
    _hFile(INVALID_HANDLE_VALUE),
    _hIntf(NULL)
{
}


TabletReader::~TabletReader() {
    close();
}


bool TabletReader::open(const QString& devicePath, DWORD& dwError) {
    WCHAR szDevicePath[_MAX_PATH] = { 0 };
    QByteArray buffer(128, 0);
    ZeroMemory(szDevicePath, _MAX_PATH * sizeof(WCHAR));
    devicePath.toWCharArray(szDevicePath);

    close();
    _hFile = CreateFile(szDevicePath,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED, NULL);
    if (_hFile == INVALID_HANDLE_VALUE)
        goto error_exit;
    BOOL bRet = WinUsb_Initialize(_hFile, &_hIntf);
    if (!bRet)
        goto error_exit;

    ULONG ulReaded = 0;
    bRet = WinUsb_GetDescriptor(_hIntf, USB_DEVICE_DESCRIPTOR_TYPE,
        0, 0, (PUCHAR)&_devDesc, sizeof(USB_DEVICE_DESCRIPTOR), &ulReaded);
    if (!bRet)
        goto error_exit;
    bRet = WinUsb_QueryInterfaceSettings(_hIntf, 0, &_intfDesc);
    if (!bRet)
        goto error_exit;
    bRet = WinUsb_QueryPipe(_hIntf, 0, 0, &_pipeInfo);
    if (!bRet)
        goto error_exit;

    if (!sendControl(201, buffer) || !parse_201((PBYTE)buffer.data()))
        goto error_exit;
    if (!sendControl(200, buffer) || !parse_200((PBYTE)buffer.data()))
        goto error_exit;
    if (!sendControl(202, buffer) || !parse_202((PBYTE)buffer.data()))
        goto error_exit;

    _info.devicePath = devicePath;
    return true;

error_exit:
    dwError = GetLastError();
    close();
    return false;
}


void TabletReader::close() {
    if (_hIntf != NULL) {
        WinUsb_Free(_hIntf);
        _hIntf = NULL;
    }
    if (_hFile != INVALID_HANDLE_VALUE) {
        CloseHandle(_hFile);
        _hFile = INVALID_HANDLE_VALUE;
    }
}

bool TabletReader::sendControl(int nCmd, QByteArray& buffer) {
    WINUSB_SETUP_PACKET wsp;
    wsp.RequestType = 0x80;
    wsp.Request = 0x06;
    wsp.Index = 0x0409;
    wsp.Length = 0x5678;
    wsp.Value = 0x0300 | nCmd;
    buffer.fill(0);

    ULONG ulTransferred;
    BOOL bRet = WinUsb_ControlTransfer(_hIntf,
        wsp,
        (PUCHAR)buffer.data(),
        buffer.size(),
        &ulTransferred,
        NULL);
    return bRet ? true : false;
}


bool TabletReader::parse_201(const BYTE* pData) {
    QString value = QString::fromWCharArray((PWSTR)(pData + 2));
    QStringList parts = value.split('_');
    if (parts.size() != 3)
        return false;

    _info.oem = parts[0];
    _info.model = parts[1];
    _info.fixtureDate = parts[2];
    return true;
}


bool TabletReader::parse_200(const BYTE* pBuf) {
    const BYTE* pData = pBuf + 2;
    int cx = (pData[2] << 16) | (pData[1] << 8) | pData[0];
    int cy = (pData[5] << 16) | (pData[4] << 8) | pData[3];
    _info.size = QSize(cx, cy);
    _info.maxPressure = (pData[7] << 8) | pData[6];
    _info.pBtnNum = pData[10];
    _info.hBtnNum = pData[11];
    _info.sBtnNum = pData[12];
    _info.lpi = (pData[9] << 8) | pData[8];
    _info.rate = pData[16] * 4;
    _info.isMonitor = (pData[14] & 0xa) != 0;
    _info.isPassive = (pData[14] & 0xc) != 0;

    return true;
}


bool TabletReader::parse_202(const BYTE* pBuf) {
    size_t maxSize = 64;
    PWSTR pStart = (PWSTR)(pBuf + 2);
    int endIndex = 63;
    for (int i = 0; i < maxSize; i++) {
        WCHAR ch = *(pStart + i);
        if (ch == 0 || ch >= 0x80) {
            endIndex = i;
            break;
        }
    }
    _info.company = QString::fromWCharArray(pStart, endIndex);
    return true;
}


void TabletReader::startRead() {
    _thread = new TabletReaderThread(this);
    connect(_thread, &QThread::finished, this, &TabletReader::onThreadFinished);
    _thread->start();
}


void TabletReader::stopRead() {
    close();
    if (_thread) {
        _thread->requestInterruption();
    }
}


bool TabletReader::read(QByteArray& buffer, PULONG ulReaded) {
    BOOL bRet = WinUsb_ReadPipe(_hIntf,
        _pipeInfo.PipeId,
        (PUCHAR)buffer.data(),
        buffer.size(),
        ulReaded,
        NULL);
    return bRet ? true : false;
}


void TabletReader::onThreadFinished() {
    if (_thread) {
        _thread->deleteLater();
        _thread = nullptr;
    }
    deleteLater();
}


bool TabletReader::waitQuit(int milliseconds) {
    if (_thread) {
        return _thread->wait(milliseconds);
    }
    return false;
}
