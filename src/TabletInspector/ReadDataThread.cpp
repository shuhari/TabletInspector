#include "stdafx.h"
#include "ReadDataThread.h"


using namespace win32;


ReadDataThread::ReadDataThread(UsbReader* reader) 
    : _reader(reader) {

    connect(this, &QThread::finished, this, &QObject::deleteLater);
}


ReadDataThread::~ReadDataThread() {
    delete _reader;
    _reader = nullptr;
}


void ReadDataThread::run() {

    while (!isInterruptionRequested()) {
        BYTE packet[64] = { 0 };
        ZeroMemory(packet, sizeof(packet));
        UsbBuffer ub(packet, sizeof(packet));
        if (_reader->read(ub) && ub.transferred > 0) {
            QByteArray ba((char*)packet, ub.transferred);
            emit readData(ba);
        }
        else {
            DWORD dwError = LastError::current();
            emit readError(dwError);
        }
    }

    _reader->close();
}