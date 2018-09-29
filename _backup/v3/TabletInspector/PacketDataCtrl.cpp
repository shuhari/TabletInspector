#include "stdafx.h"
#include "PacketDataCtrl.h"


PacketDataCtrl::PacketDataCtrl(QWidget* parent)
    : QPlainTextEdit(parent)  {

    setReadOnly(true);
    setFont(QFont("Courier New", 10));
}


void PacketDataCtrl::addData(const QByteArray& ba) {
    auto doc = document();
    if (doc->characterCount() >= 5000) {
        clear();
    }
    QString line;
    for (int i = 0; i < ba.size(); i++) {
        uint ch = (BYTE)ba[i];
        QString sch = QString("%1 ").arg(ch, 2, 16, QChar('0'));
        line.append(sch);
    }
    // line.append("\n");
    appendPlainText(line);
    verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}