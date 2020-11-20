#pragma once
#include <QAbstractTableModel>
#include "Servcice.h"
class MyListModel : public QAbstractTableModel
{
private:
	Service& service;
public:
	MyListModel(Service& service, QObject* parent = NULL);
	~MyListModel();
	int rowCount(const QModelIndex& parent = QModelIndex{}) const override;

	int columnCount(const QModelIndex& parent = QModelIndex{}) const override;

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	Qt::ItemFlags flags(const QModelIndex& index) const override;
};