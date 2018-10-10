#include "stdafx.h"
#include "Canvas.h"
#include "Public.h"


#define MARGIN              10
#define CORNER_RADIUS       10


Canvas::Canvas(QWidget* parent) :
    QWidget(parent),
    _info(nullptr),
    _isPenDown(false),
    _currentStroke(nullptr) {

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _backBrush = QBrush(QColor::fromRgb(255, 255, 255));
    _inBrush = QBrush(QColor(192, 192, 192));
    _strokePen = QPen(QColor::fromRgb(0, 0, 0));
}


Canvas::~Canvas() {
    clearData();
}


void Canvas::notifyTablet(TabletInfo* info) {
    _info = info;
    _isPenDown = false;
    _currentStroke = nullptr;
    repaint();
}


QRect Canvas::calcRect(QRect border) {
    border.adjust(MARGIN, MARGIN, -MARGIN, -MARGIN);
    if (_info && !border.isEmpty()) {
        double xScale = (double)_info->size.width() / border.width();
        double yScale = (double)_info->size.height() / border.height();
        double scale = qMax(xScale, yScale);
        int drawWidth = (int)(_info->size.width() / scale);
        int drawHeight = (int)(_info->size.height() / scale);
        int drawX = border.left() + (border.width() - drawWidth) / 2;
        int drawY = border.top() + (border.height() - drawHeight) / 2;
        return QRect(drawX, drawY, drawWidth, drawHeight);
    }
    return QRect(0, 0, -1, -1);
}


QPoint Canvas::calcDrawPos(QRect rcDraw, QPoint pt) {
    int x = (int)((double)pt.x() / _info->size.width() * rcDraw.width());
    int y = (int)((double)pt.y() / _info->size.height() * rcDraw.height());
    return QPoint(rcDraw.left() + x,
        rcDraw.top() + y);
}


void Canvas::paintEvent(QPaintEvent* event) {
    QPainter painter;
    painter.begin(this);

    QRect rc(QPoint(0, 0), size());
    painter.fillRect(rc, _backBrush);

    if (_info) {
        QRect rcBorder = calcRect(rc);
        painter.fillRect(rcBorder, _inBrush);

        painter.setPen(_strokePen);
        for (CanvasStroke* stroke : _strokes) {
            for (int i = 0; i < stroke->size() - 1; i++) {
                QPoint pt1 = stroke->at(i);
                QPoint pt2 = stroke->at(i + 1);
                QPoint ptDraw1 = calcDrawPos(rcBorder, pt1);
                QPoint ptDraw2 = calcDrawPos(rcBorder, pt2);
                painter.drawLine(ptDraw1, ptDraw2);
            }
        }
    }

    painter.end();
}


void Canvas::notifyTabletData(const QByteArray& data) {
    DataParser parser(data);
    auto dataType = parser.dataType();

    if (dataType == DataParser::PenDown) {
        auto penPos = parser.position();
        if (!_isPenDown) { // Pen down
            _isPenDown = true;
            _currentStroke = new CanvasStroke();
            _currentStroke->append(penPos);
            _strokes.append(_currentStroke);
        } else {  // continues drawing
            if (_currentStroke) {
                _currentStroke->append(penPos);
            }
        }
    } else if (dataType == DataParser::PenUp && _isPenDown) { // Pen up
        if (_currentStroke) {
            _currentStroke->append(parser.position());
            _currentStroke = nullptr;
            repaint();
        }
        _isPenDown = false;
    }
}


void Canvas::clearTabletData() {
    clearData();
    repaint();
}


void Canvas::clearData() {
    _currentStroke = nullptr;
    for (CanvasStroke* stroke : _strokes) {
        delete stroke;
    }
    _strokes.clear();
}


void Canvas::loadData(PenDataModel* model) {
    auto stroke = new CanvasStroke();
    for (int i = 0; i < model->rowCount(); i++) {
        const QByteArray& data = model->at(i);
        QPoint point = DataParser(data).position();
        stroke->append(point);
    }
    _strokes.append(stroke);
}


void Canvas::loadData(PenDataTableModel* model) {
    auto stroke = new CanvasStroke();
    for (int i = 0; i < model->rowCount(); i++) {
        const QByteArray& data = model->at(i);
        QPoint point = DataParser(data).position();
        stroke->append(point);
    }
    _strokes.append(stroke);
}