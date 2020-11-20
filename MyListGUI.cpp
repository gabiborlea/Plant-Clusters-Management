#include "MyListGUI.h"
#include <qtableview.h>
#include "PictureDelegate.h"
MyListGUI::MyListGUI(Service& service, QWidget *parent)
	: QWidget(parent), service{service}
{
	ui.setupUi(this);

	this->table_model = new MyListModel{ service };
	this->ui.tableView->setModel(this->table_model);
	this->ui.tableView->setItemDelegate(new PictureDelegate{ service });
	this->ui.tableView->resizeColumnsToContents();
	this->ui.tableView->resizeRowsToContents();
	this->ui.tableView->verticalHeader()->hide();
}

MyListGUI::~MyListGUI()
{
}

void MyListGUI::update()
{
	this->table_model = new MyListModel{ service };
	this->ui.tableView->setModel(this->table_model);
	this->ui.tableView->setItemDelegate(new PictureDelegate{ service });
	this->ui.tableView->resizeColumnsToContents();
	this->ui.tableView->resizeRowsToContents();
	this->ui.tableView->verticalHeader()->hide();
}
