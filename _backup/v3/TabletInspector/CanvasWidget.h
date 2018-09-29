#pragma once


#include "stdafx.h"


class CanvasWidget : public QWidget
{
    Q_OBJECT
public:
    CanvasWidget(QWidget* parent = nullptr);
    virtual ~CanvasWidget() = default;
};

