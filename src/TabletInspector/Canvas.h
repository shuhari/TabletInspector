#pragma once


class Canvas : public QWidget {
    Q_OBJECT
public:
    Canvas(QWidget* parent = nullptr);
    virtual ~Canvas() = default;
};

