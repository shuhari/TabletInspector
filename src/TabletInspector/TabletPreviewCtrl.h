#pragma once


class TabletPreviewCtrl : public CWindowImpl<TabletPreviewCtrl, CStatic> {
public:
    DECLARE_WND_SUPERCLASS(NULL, WC_STATIC)

    BEGIN_MSG_MAP_EX(TabletPreviewCtrl)
        MSG_WM_PAINT(onPaint)
        DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()

    TabletPreviewCtrl();
    virtual ~TabletPreviewCtrl() = default;

    void setSize(CSize size);
    void unsetSize();
    void setPos(CPoint pt);

private:
    CSize   _size;
    CPoint  _point;
    void    onPaint(CDCHandle);

    bool    isValidSize();
    bool    isValidPoint(CPoint pt);
    void    drawPos(CDCHandle dc, CPoint pt, bool on);
};