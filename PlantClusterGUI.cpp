#include "PlantClusterGUI.h"
#include <qmessagebox.h>
#include <map>
#include <QRandomGenerator>

PlantClusterGUI::PlantClusterGUI(Service& service, QMainWindow* parent)
	: QMainWindow(parent), service(service)
{
	ui.setupUi(this);
	this->populate_list();
	this->populate_my_list();
	this->connections();
	try {
		auto current_plant_cluster = this->service.get_current();
		this->ui.CurrentPlantCluster->setText(QString::fromStdString(current_plant_cluster.get_plant_cluster_as_string()));
	}
	catch (const std::exception& exception) {}
}

PlantClusterGUI::~PlantClusterGUI()
{
}

void PlantClusterGUI::populate_list()
{
	this->ui.PlantClusterList->clear();
	std::vector<PlantCluster> plant_cluster_list = this->service.get_all_plant_clusters();
	for (const auto& plant_cluster : plant_cluster_list)
		this->ui.PlantClusterList->addItem(QString::fromStdString(plant_cluster.get_plant_cluster_as_string()));
}

void PlantClusterGUI::populate_my_list()
{
	std::string species = this->ui.SpeciesFilter->text().toStdString();
	int age_in_months = -1;

	if (this->ui.AgeFilter->text().toStdString() != "")
	{
		try
		{
			age_in_months = std::stoi(this->ui.AgeFilter->text().toStdString());
		}
		catch (std::invalid_argument const& e)
		{
			QMessageBox::critical(this, "Error", "Invalid age!");
			return;
		}
	}
	this->ui.UserList->clear();
	std::vector<PlantCluster> plant_cluster_list = this->service.filter_mylist(species, age_in_months);
		

	for (const auto& plant_cluster : plant_cluster_list)
		this->ui.UserList->addItem(QString::fromStdString(plant_cluster.get_plant_cluster_as_string()));
}

void PlantClusterGUI::populate_chart()
{

	
}

void PlantClusterGUI::connections()
{
	QObject::connect(this->ui.PlantClusterList, &QListWidget::itemSelectionChanged, [this]() {
		int selected_index = this->get_list_index();
		if (selected_index < 0)
			return;
		PlantCluster plant_cluster = this->service.get_all_plant_clusters()[selected_index];
		this->ui.CodedNameEdit->setText(QString::fromStdString(plant_cluster.get_coded_name()));
		this->ui.SpeciesEdit->setText(QString::fromStdString(plant_cluster.get_species()));
		this->ui.AgeInMonthsEdit->setText(QString::fromStdString(std::to_string(plant_cluster.get_age_in_months())));
		this->ui.DigitizedScanEdit->setText(QString::fromStdString(plant_cluster.get_digitized_scan()));

		});
	QObject::connect(this->ui.AddButton, &QPushButton::clicked, this, &PlantClusterGUI::add_plant_cluster);
	QObject::connect(this->ui.DeleteButton, &QPushButton::clicked, this, &PlantClusterGUI::delete_plant_cluster);
	QObject::connect(this->ui.UpdateButton, &QPushButton::clicked, this, &PlantClusterGUI::update_plant_cluster);
	QObject::connect(this->ui.NextButton, &QPushButton::clicked, this, &PlantClusterGUI::set_next);
	QObject::connect(this->ui.SaveButton, &QPushButton::clicked, this, &PlantClusterGUI::save_plant_cluster);
	QObject::connect(this->ui.FilterButton, &QPushButton::clicked, this, &PlantClusterGUI::populate_my_list);
	QObject::connect(this->undo_shortcut, &QShortcut::activated, this, &PlantClusterGUI::undo);
	QObject::connect(this->redo_shortcut, &QShortcut::activated, this, &PlantClusterGUI::redo);
	QObject::connect(this->ui.actionUndo, &QAction::triggered, this, &PlantClusterGUI::undo);
	QObject::connect(this->ui.actionRedo, &QAction::triggered, this, &PlantClusterGUI::redo);
	QObject::connect(this->ui.OpenListButton, &QPushButton::clicked, this, &PlantClusterGUI::mylist_view);
	QObject::connect(this->ui.OpenAppButton, &QPushButton::clicked, this, &PlantClusterGUI::open_in_app);
	

}

int PlantClusterGUI::get_list_index() const
{
	QModelIndexList selected_indexes = this->ui.PlantClusterList->selectionModel()->selectedIndexes();
	if (selected_indexes.size() == 0)
	{
		this->ui.CodedNameEdit->clear();
		this->ui.SpeciesEdit->clear();
		this->ui.AgeInMonthsEdit->clear();
		this->ui.DigitizedScanEdit->clear();
		return -1;
	}

	int selected_index = selected_indexes.at(0).row();
	return selected_index;
}

void PlantClusterGUI::add_plant_cluster()
{
	std::string coded_name = this->ui.CodedNameEdit->text().toStdString();
	std::string species = this->ui.SpeciesEdit->text().toStdString();
	int age_in_months;
	std::string digitized_scan = this->ui.DigitizedScanEdit->text().toStdString();

	try
	{
		age_in_months = std::stoi(this->ui.AgeInMonthsEdit->text().toStdString());
	}
	catch (std::invalid_argument const& e)
	{
		QMessageBox::critical(this, "Error", "Invalid age!");
		return;
	}
	try
	{
		this->service.add_plant_cluster(coded_name, species, age_in_months, digitized_scan);
		this->populate_list();
		this->populate_chart();
		int last_plant_cluster = this->service.get_all_plant_clusters().size() - 1;
		this->ui.PlantClusterList->setCurrentRow(last_plant_cluster);
	}
	catch (const std::exception& exception)
	{
		QMessageBox::critical(this, "Error", QString::fromStdString(exception.what()));
	}
}

void PlantClusterGUI::delete_plant_cluster()
{
	int selected_index = this->get_list_index();
	if (selected_index < 0)
		QMessageBox::critical(this, "Error", "No item selected");

	PlantCluster plant_cluster = this->service.get_all_plant_clusters()[selected_index];
	std::string coded_name = plant_cluster.get_coded_name();
	try
	{
		this->service.delete_plant_cluster(coded_name);
		this->populate_chart();
		this->populate_list();
	}
	catch (const RepositoryException& exception)
	{
		QMessageBox::critical(this, "Error", QString::fromStdString(exception.what()));
	}
}

void PlantClusterGUI::update_plant_cluster()
{
	std::string coded_name = this->ui.CodedNameEdit->text().toStdString();
	std::string species = this->ui.SpeciesEdit->text().toStdString();
	int age_in_months;
	std::string digitized_scan = this->ui.DigitizedScanEdit->text().toStdString();

	try
	{
		age_in_months = std::stoi(this->ui.AgeInMonthsEdit->text().toStdString());
	}
	catch (std::invalid_argument const& e)
	{
		QMessageBox::critical(this, "Error", "Invalid age!");
		return;
	}
	try
	{
		this->service.update_plant_cluster(coded_name, species, age_in_months, digitized_scan);
		this->populate_list();
		this->populate_chart();
		int last_plant_cluster = this->service.get_all_plant_clusters().size() - 1;
		this->ui.PlantClusterList->setCurrentRow(last_plant_cluster);
	}
	catch (const std::exception& exception)
	{
		QMessageBox::critical(this, "Error", QString::fromStdString(exception.what()));
	}
}

void PlantClusterGUI::set_current()
{
	try {
		auto current_plant_cluster = this->service.get_current();
		this->ui.CurrentPlantCluster->setText(QString::fromStdString(current_plant_cluster.get_plant_cluster_as_string()));
	}
	catch (const std::exception& exception)
	{
		QMessageBox::critical(this, "Error", QString::fromStdString(exception.what()));
	}
}

void PlantClusterGUI::set_next()
{
	std::string species = this->ui.SpeciesFilter->text().toStdString();
	int age_in_months = -1;

	if (this->ui.AgeFilter->text().toStdString() != "")
	{
		try
		{
			age_in_months = std::stoi(this->ui.AgeFilter->text().toStdString());
		}
		catch (std::invalid_argument const& e)
		{
			QMessageBox::critical(this, "Error", "Invalid age!");
			return;
		}
	}
	try {
		this->service.next(species, age_in_months);
		this->set_current();
	}
	catch (const std::exception& exception)
	{
		QMessageBox::critical(this, "Error", QString::fromStdString(exception.what()));
	}
}

void PlantClusterGUI::save_plant_cluster()
{
	try
	{
		std::string coded_name = this->service.get_current().get_coded_name();
		this->service.save_plant_cluster(coded_name);
		this->populate_my_list();
	}
	catch (const std::exception& exception)
	{
		QMessageBox::critical(this, "Error", QString::fromStdString(exception.what()));
	}
}

void PlantClusterGUI::open_in_app()
{
	this->service.open_in_app();
}

void PlantClusterGUI::undo()
{
	auto index = this->ui.tabWidget->currentIndex();
	if (index == 0)
	{
		try {
			this->service.undo();
			this->populate_list();
			this->populate_chart();
		}
		catch (const std::exception& exception)
		{
			QMessageBox::critical(this, "Error", QString::fromStdString(exception.what()));
		}
	}
	else if (index == 1)
	{
		try {
			this->service.undo_user();
			this->populate_my_list();
			this->populate_chart();
		}
		catch (const std::exception& exception)
		{
			QMessageBox::critical(this, "Error", QString::fromStdString(exception.what()));
		}
	}
}

void PlantClusterGUI::redo()
{
	auto index = this->ui.tabWidget->currentIndex();
	if (index == 0)
	{
		try {
			this->service.redo();
			this->populate_list();
			this->populate_chart();
		}
		catch (const std::exception& exception)
		{
			QMessageBox::critical(this, "Error", QString::fromStdString(exception.what()));
		}
	}
	else if (index == 1)
	{
		try {
			this->service.redo_user();
			this->populate_my_list();
			this->populate_chart();
		}
		catch (const std::exception& exception)
		{
			QMessageBox::critical(this, "Error", QString::fromStdString(exception.what()));
		}
	}
}

void PlantClusterGUI::mylist_view()
{

	MyListGUI* mylist_view = new MyListGUI{ this->service};
	this->service.addObserver(mylist_view);
	mylist_view->show();

}

