#pragma once


#include "stdafx.h"
#include "Resources.h"


class LogList : public QListWidget  {
public:
    LogList(QWidget* parent = nullptr);
    virtual ~LogList()= default;

    void debug(const QString& msg);
    void info(const QString& msg);
    void warn(const QString& msg);
    void error(const QString& msg);

private:
    QMap<ImageKey, QIcon>   _icons;
    QAction*                _acClear;

    void loadIcon(ImageKey key);
    void addLog(ImageKey iconKey, const QString& msg);

private slots:
    void onClear();
};

