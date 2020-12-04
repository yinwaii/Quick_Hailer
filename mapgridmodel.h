#ifndef MAPGRIDMODEL_H
#define MAPGRIDMODEL_H

#include <QAbstractListModel>

class MapGridModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit MapGridModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
};

#endif // MAPGRIDMODEL_H
