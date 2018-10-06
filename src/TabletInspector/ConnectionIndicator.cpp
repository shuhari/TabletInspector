#include "stdafx.h"
#include "ConnectionIndicator.h"
#include "QtHelper.h"
#include "Resources.h"
#include "Strings.h"


ConnectionIndicator::ConnectionIndicator(QWidget* parent) 
: QWidget(parent) {
    setFixedWidth(150);

    _icon = new QLabel();
    _text = new QLabel();

    Layout::hbox()
        .margin(0)
        .add(_icon)
        .add(_text, 1)
        .apply(this);
}


void ConnectionIndicator::setConnected(const QString& tabletName) {
    _icon->setPixmap(Resources::connected());
    _text->setText(tabletName);
}


void ConnectionIndicator::setDisconnected() {
    _icon->setPixmap(Resources::disconnected());
    _text->setText(Strings::disconnected());
}