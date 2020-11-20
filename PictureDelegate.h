#pragma once
#include <QStyledItemDelegate>
#include "Servcice.h"

class PictureDelegate : public QStyledItemDelegate
{
private:
	Service& service;
public:
	PictureDelegate(Service& service, QWidget* parent = 0);

	// these two functions need to be overridden to draw an image for each item
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};