#pragma once


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
    QString toHex(const QByteArray& data) const;

private:
    QList<QByteArray> _datas;
    enum ItemType {
        None = 0,
        PenDown = 1,
        PenBtnDown = 2,
    };
    QMap<ItemType, QBrush> _brushes;

    ItemType getItemType(const QByteArray& data) const;
};

