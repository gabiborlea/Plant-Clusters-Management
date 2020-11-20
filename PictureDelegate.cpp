#include "PictureDelegate.h"
#include <QPixmap>
#include <QPainter>
PictureDelegate::PictureDelegate(Service& service, QWidget* parent) : QStyledItemDelegate{ parent }, service{service}
{
}

void PictureDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{

	auto digitized_scan = this->service.get_all_plant_clusters()[index.row()].get_digitized_scan();

	if (index.column() != 3)
	{
		QStyledItemDelegate::paint(painter, option, index);
		return;
	}

	QPixmap pixMap(QString::fromStdString(digitized_scan));
	painter->drawPixmap(option.rect, pixMap);
}

QSize PictureDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.column() == 1)
	{
		return QSize(32, 100);
	}

	return QStyledItemDelegate::sizeHint(option, index);
}