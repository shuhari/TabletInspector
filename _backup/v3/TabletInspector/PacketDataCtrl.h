#pragma once

class PacketDataCtrl : public QPlainTextEdit
{
    Q_OBJECT
public:
    PacketDataCtrl(QWidget* parent = nullptr);
    virtual ~PacketDataCtrl() = default;

    void addData(const QByteArray& ba);
};

