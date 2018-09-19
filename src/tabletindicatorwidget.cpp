#include "tabletindicatorwidget.h"

TabletIndicatorWidget::TabletIndicatorWidget() {
    setFixedWidth(150);

    _icon = new QLabel();
    _text = new QLabel();

    auto layout = new QHBoxLayout();
    layout->setMargin(0);
    layout->addWidget(_icon);
    layout->addWidget(_text, 1);
    setLayout(layout);
}


void TabletIndicatorWidget::setConnected(const QString& name) {
    _icon->setPixmap(QPixmap(":/images/light_on.png"));
    _text->setText(name);
}


void TabletIndicatorWidget::setDisconnected() {
    _icon->setPixmap(QPixmap(":/images/light_off.png"));
    _text->setText(tr("Disconnected"));
}
