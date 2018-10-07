#include "stdafx.h"
#include "HexPage.h"


#define MAX_ITEMS    100


HexModel::HexModel(QObject* parent) :
    QAbstractListModel(parent) {

    _brushes[None] = QBrush(QColor(255, 255, 255));
    _brushes[PenDown] = QBrush(QColor(0, 128, 0));
    _brushes[PenBtnDown] = QBrush(QColor(128, 128, 0));
}


int HexModel::rowCount(const QModelIndex &parent) const {
    return _datas.size();
}


void HexModel::add(const QByteArray& data) {
    if (_datas.size() >= MAX_ITEMS) {
        beginRemoveRows(QModelIndex(), 0, 0);
        _datas.removeFirst();
        endRemoveRows();
    }

    int count = _datas.size();
    beginInsertRows(QModelIndex(), count, count+1);
    _datas.append(data);
    endInsertRows();
}


QVariant HexModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();
    if (index.row() < 0 || index.row() >= _datas.size())
        return QVariant();

    QByteArray data = _datas[index.row()];
    if (role == Qt::DisplayRole) {
        char hexStr[200] = { 0 };
        ZeroMemory(hexStr, sizeof(hexStr));
        char* pNext = hexStr;
        for (BYTE b : data) {
            wsprintfA(pNext, "%02X ", b);
            pNext += 3;
        }
        return QString(hexStr);
    } else if (role == Qt::BackgroundRole) {
        ItemType itemType = getItemType(data);
        return _brushes[itemType];
    }
    return QVariant();
}


HexModel::ItemType HexModel::getItemType(const QByteArray& data) const {
    if (data.size() < 2)
        return None;
    if (data[0] != 0x08)
        return None;
    BYTE flag = data[1];
    if ((flag & 0x81) == 0x81)
        return PenDown;
    if ((flag & 0x82) == 0x82 || (flag & 0x84) == 0x84)
        return PenBtnDown;
    return None;
}


HexPage::HexPage(QWidget* parent) :
    QListView(parent) {

    setFont(QFont("Courier New", 10));
    this->setUniformItemSizes(true);

    _model = new HexModel(this);
    setModel(_model);
}


HexPage::~HexPage() {
}


void HexPage::addData(const QByteArray& data) {
    _model->add(data);
    scrollToBottom();
}