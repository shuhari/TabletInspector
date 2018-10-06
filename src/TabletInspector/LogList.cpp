#include "stdafx.h"
#include "LogList.h"
#include "Resources.h"
#include "Strings.h"


#define MAX_ITEMS               1000


LogList::LogList(QWidget* parent) 
    : QListWidget(parent) {

    _icons[logDebug] = Resources::debug();
    _icons[logInfo] = Resources::info();
    _icons[logWarn] = Resources::warn();
    _icons[logError] = Resources::error();

    QAction* acClear = new QAction(Strings::clear(), this);
    connect(acClear, &QAction::triggered, this, &QListWidget::clear);
    addAction(acClear);
    setContextMenuPolicy(Qt::ActionsContextMenu);
}


void LogList::debug(const QString& msg) {
    addLog(logDebug, msg);
}


void LogList::info(const QString& msg) {
    addLog(logInfo, msg);
}


void LogList::warn(const QString& msg) {
    addLog(logWarn, msg);
}


void LogList::error(const QString& msg) {
    addLog(logError, msg);
}


void LogList::addLog(LogLevel level, const QString& msg) {
    if (count() >= MAX_ITEMS)
        model()->removeRow(0);

    QTime now = QTime::currentTime();
    QString sTime = now.toString("HH:mm:ss");
    QString content = QString("[%1] %2").arg(sTime).arg(msg);
    QIcon icon = _icons[level];
    auto item = new QListWidgetItem(icon, content, this);
    addItem(item);
}
