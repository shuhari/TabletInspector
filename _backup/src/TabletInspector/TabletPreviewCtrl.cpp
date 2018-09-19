#include "stdafx.h"
#include "TabletPreviewCtrl.h"


#define BACK_COLOR  RGB(96, 96, 96)
#define FORE_COLOR  RGB(0, 0, 255)
#define RADIUS 4



TabletPreviewCtrl::TabletPreviewCtrl() {
}


void TabletPreviewCtrl::onPaint(CDCHandle) {
    CRect rcClient;
    GetClientRect(rcClient);
    CPaintDC dc(*this);
    dc.FillSolidRect(rcClient, BACK_COLOR);
    if (isValidSize() && isValidPoint(_point)) {
        drawPos((HDC)dc, _point, true);
    }
}


void TabletPreviewCtrl::setSize(CSize size) {
    _size = size;
    InvalidateRect(NULL, TRUE);
}


void TabletPreviewCtrl::unsetSize() {
    _size.SetSize(0, 0);
    InvalidateRect(NULL, TRUE);
}


bool TabletPreviewCtrl::isValidSize() {
    return _size.cx > 0 && _size.cy > 0;
}


bool TabletPreviewCtrl::isValidPoint(CPoint pt) {
    if (!isValidSize())
        return false;
    return pt.x > 0 && pt.y < _size.cx &&
        pt.y > 0 && pt.y < _size.cy;
}


void TabletPreviewCtrl::setPos(CPoint pt) {
    CDCHandle dc = GetDC();
    if (isValidPoint(_point))
        drawPos(dc, _point, false);
    if (isValidPoint(pt))
        drawPos(dc, pt, true);
    ReleaseDC(dc);
    _point = pt;
}


void TabletPreviewCtrl::drawPos(CDCHandle dc, CPoint pt, bool on) {
    if (!isValidSize())
        return;

    CRect rcClient;
    GetClientRect(rcClient);
    int x = (int)((float)pt.x / _size.cx * rcClient.Width());
    int y = (int)((float)pt.y / _size.cy * rcClient.Height());
    CRect rc(x - RADIUS, y - RADIUS,
        x + RADIUS, y + RADIUS);
    dc.FillSolidRect(rc, BACK_COLOR);
    if (on) {
        CBrush foreBrush;
        foreBrush.CreateSolidBrush(FORE_COLOR);
        CBrush oldBrush = dc.SelectBrush(foreBrush);
        dc.Ellipse(rc);
        dc.SelectBrush(oldBrush);
        foreBrush.DeleteObject();
    }
}