#pragma once


#include "UsbReader.h"


class ReadDataThread : public QThread
{
    Q_OBJECT
public:
    ReadDataThread(UsbReader* reader);
    virtual ~ReadDataThread();

signals:
    void readData(const QByteArray& data);
    void readError(quint32 dwError);

protected:
    void run() override;

private:
    UsbReader*   _reader;
};

