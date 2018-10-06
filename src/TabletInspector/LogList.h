#pragma once


class LogList : public QListWidget {
    Q_OBJECT
public:
    LogList(QWidget* parent = nullptr);
    virtual ~LogList() = default;

    void debug(const QString& msg);
    void info(const QString& msg);
    void warn(const QString& msg);
    void error(const QString& msg);

private:
    enum LogLevel { logDebug, logInfo, logWarn, logError };
    QMap<LogLevel, QIcon> _icons;

    void addLog(LogLevel level, const QString& msg);
};

