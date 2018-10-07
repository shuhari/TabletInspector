#pragma once


class HexModel : public QAbstractListModel {
    Q_OBJECT
public:
    HexModel(QObject* parent = nullptr);
    virtual ~HexModel() = default;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void add(const QByteArray& data);

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


class HexPage : public QListView
{
    Q_OBJECT
public:
    HexPage(QWidget* parent = nullptr);
    virtual ~HexPage();

    void addData(const QByteArray& data);

private:
    HexModel*           _model;
};

