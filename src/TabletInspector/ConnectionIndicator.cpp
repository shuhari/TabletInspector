#include "stdafx.h"
#include "ConnectionIndicator.h"
#include "Resources.h"


ConnectionIndicator::ConnectionIndicator(QWidget* parent)
    : QWidget(parent)  {

    setFixedWidth(150);

    _icon = new QLabel();
    _text = new QLabel();
    
    _connectedPixmap = ImageRegistry::pixmap(ImageKey::connected);
    _disconnectedPixmap = ImageRegistry::pixmap(ImageKey::disconnected);

    auto hbox = new QHBoxLayout();
    hbox->setMargin(0);
    hbox->addWidget(_icon);
    hbox->addWidget(_text, 1);
    setLayout(hbox);
}


void ConnectionIndicator::setConnected(const QString& tabletName) {
    _icon->setPixmap(_connectedPixmap);
    _text->setText(tabletName);
}


void ConnectionIndicator::setDisconnected() {
    _icon->setPixmap(_disconnectedPixmap);
    _text->setText(tr("Disconnected"));
}