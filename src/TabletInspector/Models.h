#pragma once


#include "Public.h"


class TabletInfoModel : public QAbstractTableModel {
    Q_OBJECT
public:
    TabletInfoModel(QObject* parent = nullptr);
    virtual ~TabletInfoModel() = default;

    void setInfo(TabletInfo* info);

    enum Props {
        oem = 0,
        model = 1,
        fixtureDate = 2,
        company = 3,
        size = 4,
        maxPressure = 5,
        pBtnNum = 6,
        hBtnNum = 7,
        sBtnNum = 8,
        lpi = 9,
        rate = 10,
        isMonitor = 11,
        isPassive = 12,

        MaxCount = 13,
    };

    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    TabletInfo*     _info;
};


class PenDataModel : public QAbstractListModel {
    Q_OBJECT
public:
    PenDataModel(QObject* parent = nullptr);
    virtual ~PenDataModel() = default;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void add(const QByteArray& data);
    void clear();
    bool getAnalyticsRange(QModelIndex index, int& nMin, int& nMax);
    PenDataModel* slice(int nMin, int nMax);
    const QByteArray& at(int row);

private:
    QList<QByteArray> _datas;
    enum ItemType {
        Default = 0,
        Invalid = 1,
        PenDown = 2,
        PenUp = 3,
        PenBtnDown = 4,
        Touch = 5,
        Slider = 6,
    };
    QMap<ItemType, QBrush> _brushes;

    ItemType getItemType(const QByteArray& data) const;
};


class PenDataTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    PenDataTableModel(PenDataModel* srcModel, int nMin, int nMax, QObject* parent = nullptr);
    virtual ~PenDataTableModel();

    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    const QByteArray& at(int row);

private:
    enum Props {
        raw = 0,
        x = 1,
        y = 2,
        pressure = 3,
        ax = 4,
        ay = 5,
        altitude = 6,
        azimuth = 7,

        MaxCols = 8,
    };
    QList<QByteArray> _datas;
};
