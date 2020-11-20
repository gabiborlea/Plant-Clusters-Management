#pragma once

#include <QWidget>
#include "ui_PlantClusterGUI.h"
#include "Servcice.h"
#include <QShortcut>
#include "MyListGUI.h"

class PlantClusterGUI : public QMainWindow
{
	Q_OBJECT
private:
	Service& service;
	QShortcut* undo_shortcut = new QShortcut(QKeySequence("Ctrl+Z"), this);
	QShortcut* redo_shortcut = new QShortcut(QKeySequence("Ctrl+Y"), this);
public:
	PlantClusterGUI(Service& service, QMainWindow* parent = Q_NULLPTR);
	~PlantClusterGUI();

private:
	Ui::MainWindow ui;
	void populate_list();
	void populate_my_list();
	void populate_chart();
	void connections();
	int get_list_index() const;
	void add_plant_cluster();
	void delete_plant_cluster();
	void update_plant_cluster();
	void set_current();
	void set_next();
	void save_plant_cluster();
	void open_in_app();
	void undo();
	void redo();
	void mylist_view();
};
