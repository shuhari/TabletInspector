#include "stdafx.h"
#include "LogList.h"


#define MAX_ITEMS               1000


LogList::LogList(QWidget* parent)
    : QListWidget(parent)  {
    
    setFont(QFont("Courier New", 9));

    loadIcon(ImageKey::debug);
    loadIcon(ImageKey::info);
    loadIcon(ImageKey::warn);
    loadIcon(ImageKey::error);

    _acClear = new QAction(tr("Clear"), this);
    connect(_acClear, &QAction::triggered, this, &LogList::onClear);
    addAction(_acClear);
    setContextMenuPolicy(Qt::ActionsContextMenu);
}


void LogList::loadIcon(ImageKey key) {
    _icons[key] = ImageRegistry::icon(key);
}


void LogList::debug(const QString& msg) {
    addLog(ImageKey::debug, msg);
}


void LogList::info(const QString& msg) {
    addLog(ImageKey::info, msg);
}


void LogList::warn(const QString& msg) {
    addLog(ImageKey::warn, msg);
}


void LogList::error(const QString& msg) {
    addLog(ImageKey::error, msg);
}


void LogList::addLog(ImageKey iconKey, const QString& msg) {
    QTime now = QTime::currentTime();
    QString sTime = now.toString("HH:mm:ss");
    QString content = QString("[%1] %2").arg(sTime).arg(msg);
    QIcon icon = _icons[iconKey];

    if (count() >= MAX_ITEMS)
        model()->removeRow(0);
    auto item = new QListWidgetItem(icon, content, this);
    addItem(item);
}


void LogList::onClear() {
    clear();
}
