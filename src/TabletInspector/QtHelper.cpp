#include "stdafx.h"
#include "QtHelper.h"


SubMenu::SubMenu(QMenuBar* parent, const QString& text) 
    : _parent(parent) {
    
    _menu = new QMenu(text);
    parent->addMenu(_menu);
}


SubMenu& SubMenu::action(QAction* action) {
    _menu->addAction(action);
    return *this;
}


SubMenu& SubMenu::separator() {
    _menu->addSeparator();
    return *this;
}


VBoxLayoutDescriptor::VBoxLayoutDescriptor() {
    _layout = new QVBoxLayout();
}


VBoxLayoutDescriptor& VBoxLayoutDescriptor::add(QWidget* widget, int stretch) {
    _layout->addWidget(widget, stretch);
    return *this;
}


VBoxLayoutDescriptor& VBoxLayoutDescriptor::add(QLayout* layout, int stretch) {
    _layout->addLayout(layout, stretch);
    return *this;
}


VBoxLayoutDescriptor& VBoxLayoutDescriptor::margin(int margin) {
    _layout->setMargin(margin);
    return *this;
}


VBoxLayoutDescriptor& VBoxLayoutDescriptor::apply(QWidget* parent) {
    parent->setLayout(_layout);
    return *this;
}


HBoxLayoutDescriptor::HBoxLayoutDescriptor() {
    _layout = new QHBoxLayout();
}


HBoxLayoutDescriptor& HBoxLayoutDescriptor::add(QWidget* widget, int stretch) {
    _layout->addWidget(widget, stretch);
    return *this;
}


HBoxLayoutDescriptor& HBoxLayoutDescriptor::add(QLayout* layout, int stretch) {
    _layout->addLayout(layout, stretch);
    return *this;
}


HBoxLayoutDescriptor& HBoxLayoutDescriptor::margin(int margin) {
    _layout->setMargin(margin);
    return *this;
}

HBoxLayoutDescriptor& HBoxLayoutDescriptor::apply(QWidget* parent) {
    parent->setLayout(_layout);
    return *this;
}


VBoxLayoutDescriptor Layout::vbox() {
    return VBoxLayoutDescriptor();
}


HBoxLayoutDescriptor Layout::hbox() {
    return HBoxLayoutDescriptor();
}
