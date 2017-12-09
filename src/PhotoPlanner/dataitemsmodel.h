#ifndef DATAITEMSMODEL_H
#define DATAITEMSMODEL_H

#include <QObject>
#include <QList>
#include <QAbstractListModel>

template <typename TQDataItem>
class QDataItemsModel : public QAbstractListModel {
protected:
    using QDataItemsModelDataItem = TQDataItem;
    QDataItemsModel(QObject *parent = nullptr) : QAbstractListModel(parent) { }

public:
    void AddItem(QDataItemsModelDataItem *itemdata) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        dataitems_ << itemdata;
        endInsertRows();
    }

    int rowCount(const QModelIndex & parent = QModelIndex()) const {
        Q_UNUSED(parent);
        return dataitems_.count();
    }

    QDataStream &Read(QDataStream &stream) {
        int size = 0;
        stream >> size;
        for (int i = 0; i < size; i++) {
            auto item = new QDataItemsModelDataItem(this);
            item->Read(stream);
            AddItem(item);
        }
        return stream;
    }

    QDataStream &Write(QDataStream &stream) const {
        stream << dataitems_.count();
        for (auto &&item : dataitems_) {
            item->Write(stream);
        }
        return stream;
    }

protected:
    QList<QDataItemsModelDataItem *> dataitems_;
};

#endif // DATAITEMSMODEL_H
