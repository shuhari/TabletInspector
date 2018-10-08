#include "stdafx.h"
#include "Models.h"


#define MAX_ITEMS    300


PenDataModel::PenDataModel(QObject* parent) :
    QAbstractListModel(parent) {

    _brushes[Default] = QBrush(QColor(255, 255, 255));
    _brushes[Invalid] = QBrush(QColor(255, 128, 128));
    _brushes[PenDown] = QBrush(QColor(0, 128, 0));
    _brushes[PenUp] = QBrush(QColor(255, 255, 255));
    _brushes[PenBtnDown] = QBrush(QColor(255, 255, 128));
    _brushes[Touch] = QBrush(QColor(128, 255, 255));
    _brushes[Slider] = QBrush(QColor(255, 128, 255));
}


int PenDataModel::rowCount(const QModelIndex &parent) const {
    return _datas.size();
}


void PenDataModel::add(const QByteArray& data) {
    if (_datas.size() >= MAX_ITEMS) {
        beginRemoveRows(QModelIndex(), 0, 0);
        _datas.removeFirst();
        endRemoveRows();
    }

    int count = _datas.size();
    beginInsertRows(QModelIndex(), count, count + 1);
    _datas.append(data);
    endInsertRows();
}


void PenDataModel::clear() {
    beginResetModel();
    _datas.clear();
    endResetModel();
}


QVariant PenDataModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();
    if (index.row() < 0 || index.row() >= _datas.size())
        return QVariant();

    QByteArray data = _datas[index.row()];
    if (role == Qt::DisplayRole) {
        return toHex(data);
    } else if (role == Qt::BackgroundRole) {
        ItemType itemType = getItemType(data);
        return _brushes[itemType];
    } else if (role == Qt::UserRole) {
        return _datas[index.row()];
    }
    
    return QVariant();
}


PenDataModel::ItemType PenDataModel::getItemType(const QByteArray& data) const {
    if (data.size() < 2)
        return Invalid;
    if (data[0] != 0x08)
        return Invalid;
    BYTE flag = data[1];
    if ((flag & 0x80) != 0x80)
        return Invalid;

    if (flag == 0xE0)
        return Touch;
    if (flag == 0xF0)
        return Slider;

    if ((flag & 0x81) == 0x81)
        return PenDown;
    if ((flag & 0x82) == 0x82 || (flag & 0x84) == 0x84)
        return PenBtnDown;
    if (flag == 0x80)
        return PenUp;

    return Default;
}


bool PenDataModel::getAnalyticsRange(QModelIndex index, int& nMin, int& nMax) {
    nMin = index.row();
    nMax = index.row();

    for (int i = index.row() - 1; i >= 0; i--) {
        QByteArray& data = _datas[i];
        ItemType itemType = getItemType(data);
        if (itemType == Default || itemType == Invalid) {
            nMin = i + 1;
            break;
        }
    }

    for (int i = index.row() + 1; i < _datas.size(); i++) {
        QByteArray& data = _datas[i];
        ItemType itemType = getItemType(data);
        if (itemType == Default || itemType == Invalid) {
            nMax = i;
            break;
        }
    }

    return nMax >= nMin;
}



PenDataModel* PenDataModel::slice(int nMin, int nMax) {
    nMin = qMax(0, nMin);
    nMax = qMin(_datas.size() - 1, nMax);

    auto result = new PenDataModel();
    for (int i = nMin; i <= nMax; i++) {
        result->_datas.append(_datas[i]);
    }
    return result;
}


QString PenDataModel::toHex(const QByteArray& data) const {
    char hexStr[200] = { 0 };
    ZeroMemory(hexStr, sizeof(hexStr));
    char* pNext = hexStr;
    for (BYTE b : data) {
        wsprintfA(pNext, "%02X ", b);
        pNext += 3;
    }
    return QString(hexStr);
}


const QByteArray& PenDataModel::at(int row) {
    return _datas[row];
}
