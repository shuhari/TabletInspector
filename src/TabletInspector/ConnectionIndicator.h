#pragma once
class ConnectionIndicator : public QWidget {
    Q_OBJECT
public:
    ConnectionIndicator(QWidget* parent = nullptr);
    virtual ~ConnectionIndicator() = default;

    void setConnected(const QString& tabletName);
    void setDisconnected();

private:
    QLabel*          _icon;
    QLabel*          _text;
};

