#include "MyListModel.h"

MyListModel::MyListModel(Service& service, QObject* parent) : QAbstractTableModel{ parent }, service{service}
{
}

MyListModel::~MyListModel()
{
}

int MyListModel::rowCount(const QModelIndex& parent) const
{
	return this->service.get_user_list().size();
}

int MyListModel::columnCount(const QModelIndex& parent) const
{
	return 4;
}

QVariant MyListModel::data(const QModelIndex& index, int role) const
{

	int row = index.row();
	int column = index.column();

	std::vector<PlantCluster> plant_clusters = this->service.get_user_list();

	if (row == plant_clusters.size())
	{
		return QVariant();
	}

	PlantCluster plant_cluster = plant_clusters[row];

	if (role == Qt::DisplayRole)
	{
		switch (column)
		{
		case 0:
			return QString::fromStdString(plant_cluster.get_coded_name());
		case 1:
			return QString::fromStdString(plant_cluster.get_species());
		case 2:
			return QString::fromStdString(std::to_string(plant_cluster.get_age_in_months()));
		case 3:
			return QString::fromStdString(plant_cluster.get_digitized_scan());
		default:
			break;
		}
	}
	return QVariant{};
}

QVariant MyListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal)
		{
			switch (section)
			{
			case 0:
				return QString{ "Coded name" };
			case 1:
				return QString{ "Species" };
			case 2:
				return QString{ "Age in months" };
			case 3:
				return QString{ "Digitized scan" };
			default:
				break;
			}
		}
	}
	return QVariant{};
}

Qt::ItemFlags MyListModel::flags(const QModelIndex& index) const
{
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
