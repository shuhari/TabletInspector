#pragma once


class CanvasView : public CWindowImpl<CanvasView> {
public:
    DECLARE_WND_CLASS(L"CanvasView")

    BEGIN_MSG_MAP_EX(CanvasView)
    END_MSG_MAP()

    CanvasView();
    virtual ~CanvasView() = default;
};
