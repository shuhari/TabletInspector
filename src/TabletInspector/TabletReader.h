#pragma once

#include "Public.h"


class TabletReaderThread : public QThread {
    Q_OBJECT

public:
    TabletReaderThread(QObject* parent);
    virtual ~TabletReaderThread() = default;

protected:
    void run() override;
};


class TabletReader : public QObject {
    Q_OBJECT
public:
    TabletReader(QObject* parent);
    virtual ~TabletReader();

    inline TabletInfo& info() { return _info; }

    bool open(const QString& devicePath);
    void close();
    bool read(QByteArray& buffer, PULONG ulReaded);

    void startRead();
    void stopRead();

signals:
    void tabletReadData(const QByteArray& buffer);
    void tabletReadError(DWORD dwError);

private:
    TabletInfo                  _info;
    QThread*                    _thread;

    HANDLE                      _hFile;
    WINUSB_INTERFACE_HANDLE     _hIntf;
    USB_DEVICE_DESCRIPTOR       _devDesc;
    USB_INTERFACE_DESCRIPTOR    _intfDesc;
    WINUSB_PIPE_INFORMATION     _pipeInfo;

    bool sendControl(int nCmd, QByteArray& buffer);
    bool parse_201(const BYTE* pData);
    bool parse_200(const BYTE* pData);
    bool parse_202(const BYTE* pData);

private slots:
    void        onThreadFinished();
};

