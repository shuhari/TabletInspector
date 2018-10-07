#pragma once


class SubMenu {
public:
    SubMenu(QMenuBar* parent, const QString& text);

    SubMenu& action(QAction* action);
    SubMenu& separator();

private:
    QMenuBar    _parent;
    QMenu*      _menu;
};


class VBoxLayoutDescriptor {
public:
    VBoxLayoutDescriptor();
    VBoxLayoutDescriptor(const VBoxLayoutDescriptor&) = default;
    VBoxLayoutDescriptor& operator=(const VBoxLayoutDescriptor&) = default;

    inline QVBoxLayout* layout() { return _layout; }

    VBoxLayoutDescriptor& add(QWidget* widget, int stretch = 0);
    VBoxLayoutDescriptor& add(QLayout* layout, int stretch = 0);
    VBoxLayoutDescriptor& margin(int margin);
    VBoxLayoutDescriptor& addStretch(int stretch);
    VBoxLayoutDescriptor& apply(QWidget* parent);

private:
    QVBoxLayout*     _layout;
};


class HBoxLayoutDescriptor {
public:
    HBoxLayoutDescriptor();
    HBoxLayoutDescriptor(const HBoxLayoutDescriptor&) = default;
    HBoxLayoutDescriptor& operator=(const HBoxLayoutDescriptor&) = default;

    inline QHBoxLayout* layout() { return _layout; }

    HBoxLayoutDescriptor& add(QWidget* widget, int stretch = 0);
    HBoxLayoutDescriptor& add(QLayout* layout, int stretch = 0);
    HBoxLayoutDescriptor& margin(int margin);
    HBoxLayoutDescriptor& apply(QWidget* parent);

private:
    QHBoxLayout*    _layout;
};


class GridLayoutDescriptor {
public:
    GridLayoutDescriptor();
    GridLayoutDescriptor(const GridLayoutDescriptor&) = default;
    GridLayoutDescriptor& operator=(const GridLayoutDescriptor&) = default;

    GridLayoutDescriptor& add(QWidget* widget, int row, int col,
        int rowSpan = 0, int colSpan = 0, Qt::Alignment align = Qt::AlignLeft);
    GridLayoutDescriptor& add(QLayout* layout, int row, int col,
        int rowSpan = 0, int colSpan = 0, Qt::Alignment align = Qt::AlignLeft);
    GridLayoutDescriptor& apply(QWidget* parent);
    inline QGridLayout* layout() { return _layout; }

private:
    QGridLayout*        _layout;
};


class Layout {
public:
    static VBoxLayoutDescriptor vbox();
    static HBoxLayoutDescriptor hbox();
    static GridLayoutDescriptor grid();
};