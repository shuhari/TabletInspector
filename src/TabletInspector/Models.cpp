#include "stdafx.h"
#include "Models.h"
#include "Strings.h"


#define MAX_ITEMS    300


TabletInfoModel::TabletInfoModel(QObject* parent) :
    QAbstractTableModel(parent),
    _info(nullptr) {
}


void TabletInfoModel::setInfo(TabletInfo* info) {
    beginResetModel();
    _info = info;
    endResetModel();
}


int TabletInfoModel::columnCount(const QModelIndex& parent) const {
    return 1;
}


int TabletInfoModel::rowCount(const QModelIndex &parent) const {
    return _info ? Props::MaxCount : 0;
}


QVariant TabletInfoModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || !_info)
        return QVariant();
    int row = index.row(),
        col = index.column();
    if (row < 0 || row >= Props::MaxCount ||
        col < 0 || col >= 2)
        return QVariant();

    if (col == 0 && role == Qt::DisplayRole) {
        switch ((Props)row) {
        case oem: return _info->oem;
        case model: return _info->model;
        case fixtureDate: return _info->fixtureDate;
        case company: return _info->company;
        case size: return _info->sizeStr();
        case maxPressure: return formatInt(_info->maxPressure);
        case pBtnNum: return formatInt(_info->pBtnNum);
        case hBtnNum: return formatInt(_info->hBtnNum);
        case sBtnNum: return formatInt(_info->sBtnNum);
        case lpi: return formatInt(_info->lpi);
        case rate: return formatInt(_info->rate);
        case isMonitor: return _info->isMonitor ? Strings::trueStr() : Strings::falseStr();
        case isPassive: return _info->isPassive ? Strings::passive() : Strings::unPassive();
        }
    }

    return QVariant();
}


QVariant TabletInfoModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Vertical) {
        if (role == Qt::DisplayRole) {
            switch ((Props)section) {
            case oem: return Strings::oem();
            case model: return Strings::model();
            case fixtureDate: return Strings::fixtureDate();
            case company: return Strings::company();
            case size: return Strings::size();
            case maxPressure: return Strings::maxPressure();
            case pBtnNum: return Strings::pBtnNum();
            case hBtnNum: return Strings::hBtnNum();
            case sBtnNum: return Strings::sBtnNum();
            case lpi: return Strings::lpi();
            case rate: return Strings::rate();
            case isMonitor: return Strings::isMonitor();
            case isPassive: return Strings::isPassive();
            }
        }
    }
    return QVariant();
}


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


const QByteArray& PenDataModel::at(int row) {
    return _datas[row];
}


PenDataTableModel::PenDataTableModel(PenDataModel* srcModel,
    int nMin, int nMax, QObject* parent) :
    QAbstractTableModel(parent) {
    nMin = qMax(0, nMin);
    nMax = qMin(srcModel->rowCount() - 1, nMax);

    for (int i = nMin; i <= nMax; i++) {
        _datas.append(srcModel->at(i));
    }
}


PenDataTableModel::~PenDataTableModel() {
    _datas.clear();
}


int PenDataTableModel::columnCount(const QModelIndex& parent) const {
    return Props::MaxCols;
}


int PenDataTableModel::rowCount(const QModelIndex &parent) const {
    return _datas.size();
}


QVariant PenDataTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();
    int row = index.row(),
        col = index.column();
    if (row < 0 || row >= _datas.size() ||
        col < 0 || col >= MaxCols)
        return QVariant();

    const QByteArray& data = _datas[row];
    DataParser parser(data);
    QPoint pos = parser.position();
    int pressure = parser.pressure();
    QPoint tilt = parser.tilt();
    QPoint polar = parser.convertToPolar(tilt);
    if (role == Qt::DisplayRole) {
        switch ((Props)col) {
        case raw: return toHex(data);
        case x: return formatInt(pos.x());
        case y: return formatInt(pos.y());
        case Props::pressure: return formatInt(pressure);
        case ax: return formatInt(tilt.x());
        case ay: return formatInt(tilt.y());
        case altitude: return formatInt(polar.x());
        case azimuth: return formatInt(polar.y());
        }
    }

    return QVariant();
}


QVariant PenDataTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch ((Props)section) {
            case raw: return Strings::rawData();
            case x: return Strings::x();
            case y: return Strings::y();
            case pressure: return Strings::pressure();
            case ax: return Strings::ax();
            case ay: return Strings::ay();
            case altitude: return Strings::altitude();
            case azimuth: return Strings::azimuth();
            }
        }
    } else if (orientation == Qt::Vertical) {
        if (role == Qt::DisplayRole) {
            return QString("%1").arg(section + 1);
        }
    }
    return QVariant();
}


const QByteArray& PenDataTableModel::at(int row) {
    return _datas[row];
}
