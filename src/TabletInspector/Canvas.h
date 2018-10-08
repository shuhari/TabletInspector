#pragma once


#include "Public.h"


class CanvasStroke : public QList<QPoint> {
public:
};


class Canvas : public QWidget,
    public ITabletAwareWidget {
    Q_OBJECT
public:
    Canvas(QWidget* parent = nullptr);
    virtual ~Canvas();

    void notifyTablet(TabletInfo* info) override;
    void notifyTabletData(const QByteArray& data) override;
    void clearTabletData() override;

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    TabletInfo*     _info;
    QBrush          _backBrush;
    QBrush          _inBrush;
    QPen            _strokePen;
    bool            _isPenDown;
    CanvasStroke*   _currentStroke;
    QList<CanvasStroke*> _strokes;

    QRect calcRect(QRect border);
    QPoint calcDrawPos(QRect rcDraw, QPoint pt);
    void  clearData();
};

