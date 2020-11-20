#include "Servcice.h"
#include <assert.h>
#include <Windows.h>
#include <winuser.h>
#include <memory>
#include <exception>

Service::Service(Repository* repository)
{
	this->repository = repository;
	this->user_list = new InMemoryRepository();
	this->iterated_list = this->repository->get_plant_clusters();
	iterator_position = 0;
}

Service::Service(std::string config)
{
	std::ifstream file_in(config);
	std::string line;
	while (std::getline(file_in, line))
	{
		size_t space = line.find_first_of(":");
		std::string container = line.substr(0, space);
		std::string file_name = line.substr(space + 1, line.size());

		if (container == "fileLocation")
		{
			if (file_name == "inMemory")
				this->repository = new InMemoryRepository();
			else
				this->repository = new FileRepository();
				this->repository->set_file_name(file_name);
		}
		if (container == "mylistLocation")
		{
			if (file_name == "inMemory")
				this->user_list = new InMemoryRepository();
			if (file_name.substr(file_name.size() - 3, file_name.size()) == "csv")
			{
				this->user_list = new CSVRepository();
				this->user_list->set_file_name(file_name);
				this->mylist_name = file_name;
				//ShellExecuteA(NULL, NULL, "notepad.exe", file_name.c_str(), NULL, SW_SHOWMINIMIZED);
			}

			if (file_name.substr(file_name.size() - 4, file_name.size()) == "html")
			{

				this->user_list = new HTMLRepository();
				this->user_list->set_file_name(file_name);
				this->mylist_name = file_name;
				//ShellExecuteA(NULL, NULL, "chrome.exe", mylistfile_name.c_str(), NULL, SW_SHOWMINIMIZED);
			}
		}
	}
	this->iterated_list = this->repository->get_plant_clusters();
	this->iterator_position = 0;
}

Service::~Service()
{

}

void Service::set_file_name(std::string file_name)
{

	this->repository->set_file_name(file_name);
}

void Service::set_mylist_name(std::string mylistfile_name)
{

	if (mylistfile_name.substr(mylistfile_name.size() - 3, mylistfile_name.size()) == "csv")
	{
		this->user_list = new CSVRepository();
		this->user_list->set_file_name(mylistfile_name);

		ShellExecuteA(NULL, NULL, "notepad.exe", mylistfile_name.c_str(), NULL, SW_SHOWMINIMIZED);
	}

	if (mylistfile_name.substr(mylistfile_name.size() - 4, mylistfile_name.size()) == "html")
	{
		
		this->user_list = new HTMLRepository();
		this->user_list->set_file_name(mylistfile_name);
		ShellExecuteA(NULL, NULL, "chrome.exe", mylistfile_name.c_str(), NULL, SW_SHOWMINIMIZED);
	}
}

void Service::add_plant_cluster(const std::string coded_name, const std::string species, const int age_in_months, const std::string digitized_scan)
{

	PlantCluster plant_cluster{ coded_name, species, age_in_months, digitized_scan };
	Validator plant_cluster_validator{ plant_cluster };
	plant_cluster_validator.validation();
	this->repository->add_plant_cluster(plant_cluster);

	undo_stack.push_back(std::make_unique<ActionAdd>(plant_cluster, this->repository));
	redo_stack.clear();
}

void Service::delete_plant_cluster(const std::string coded_name)
{
	PlantCluster plant_cluster{ coded_name, "", 0, "" };
	PlantCluster deleted_plant_cluster{ this->repository->get_plant_cluster(plant_cluster) };
	this->repository->delete_plant_cluster(plant_cluster);

	undo_stack.push_back(std::make_unique<ActionDelete>(deleted_plant_cluster, this->repository));
	redo_stack.clear();
}

void Service::update_plant_cluster(const std::string coded_name, const std::string new_species, const int new_age_in_months, const std::string new_digitized_scan)
{
	PlantCluster plant_cluser{ coded_name, new_species, new_age_in_months, new_digitized_scan };
	PlantCluster previous_plant_cluster{ this->repository->get_plant_cluster(plant_cluser) };
	Validator plant_cluster_validator{ plant_cluser };
	plant_cluster_validator.validation();
	this->repository->update_plant_cluster(plant_cluser);
	undo_stack.push_back(std::make_unique<ActionUpdate>(plant_cluser, previous_plant_cluster, this->repository));
	redo_stack.clear();
}

PlantCluster Service::get_plant_cluster_by_coded_name(const std::string coded_name)
{
	PlantCluster plant_cluster{ coded_name, "", 0, "" };
	return this->repository->get_plant_cluster(plant_cluster);
}

std::vector<PlantCluster> Service::get_all_plant_clusters() const
{
	return this->repository->get_plant_clusters();
}



PlantCluster Service::get_current()
{
	if (iterated_list.size() == 0)
		throw std::exception{ "List is empty" };
	return *(this->iterated_list.begin() + this->iterator_position);
}

std::vector<PlantCluster> Service::filter_plant_clusters(std::string species, int age_in_months) const
{
	std::vector<PlantCluster> filter_result;
	auto plant_clusters = this->get_all_plant_clusters();
	for (const auto& plant_cluster :  plant_clusters)
		if (age_in_months == -1)
		{
			if (plant_cluster.get_species() == species)
				filter_result.push_back(plant_cluster);
		}
		else
		{
			if (plant_cluster.get_species() == species && plant_cluster.get_age_in_months() <= age_in_months)
				filter_result.push_back(plant_cluster);
		}
	return filter_result;
}

std::vector<PlantCluster> Service::filter_mylist(std::string species, int age_in_months) const
{
	std::vector<PlantCluster> filter_result;
	std::vector<PlantCluster> plant_clusters = this->get_user_list();
	if (species == "" && age_in_months == -1)
		return plant_clusters;
	for (const auto& plant_cluster : plant_clusters)
		if (species != "" && age_in_months == -1)
		{
			if (plant_cluster.get_species() == species)
				filter_result.push_back(plant_cluster);
		}
		else
		{
			if (plant_cluster.get_species() == species && plant_cluster.get_age_in_months() <= age_in_months)
				filter_result.push_back(plant_cluster);
		}
	return filter_result;
}

void Service::open_in_app()
{
	if (this->mylist_name.substr(this->mylist_name.size() - 3, this->mylist_name.size()) == "csv")
	{
		ShellExecuteA(NULL, NULL, "notepad.exe", this->mylist_name.c_str(), NULL, SW_SHOWMINIMIZED);
	}

	if (this->mylist_name.substr(this->mylist_name.size() - 4, this->mylist_name.size()) == "html")
	{
		ShellExecuteA(NULL, NULL, "chrome.exe", this->mylist_name.c_str(), NULL, SW_SHOWMINIMIZED);
	}
}

void Service::next(std::string species , int age_in_months )
{

	if (species == "" && age_in_months == -1)
		this->iterated_list = this->get_all_plant_clusters();
	else
		this->iterated_list = this->filter_plant_clusters(species, age_in_months);
	if (iterated_list.size() == 0)
		throw std::exception{ "List is empty" };
	auto current = iterated_list.begin() + this->iterator_position;
	auto end = iterated_list.end() - 1;

	if (current >= end)
		this->iterator_position = 0;

	else
	{
			this->iterator_position++;
	}
}

void Service::save_plant_cluster(const std::string coded_name)
{
	auto plant_cluster = this->get_plant_cluster_by_coded_name(coded_name);

	this->user_list->add_plant_cluster(plant_cluster);
	this->notify();
	undo_stack_user.push_back(std::make_unique<ActionAdd>(plant_cluster, this->user_list));
	redo_stack_user.clear();
}

void Service::erase_plant_cluster(const std::string coded_name)
{
	this->user_list->delete_plant_cluster(this->get_plant_cluster_by_coded_name(coded_name));
}

std::vector<PlantCluster> Service::get_user_list() const
{
	return this->user_list->get_plant_clusters();
}

void Service::undo()
{
	if (undo_stack.size() == 0)
		throw std::exception{ "Undo stack is empty" };
	
	auto last = std::move(undo_stack.back());
	undo_stack.pop_back();
	last->execute_undo();
	redo_stack.push_back(move(last));
}

void Service::redo()
{
	if (redo_stack.size() == 0)
		throw std::exception{ "Redo stack is empty" };
	auto last = move(redo_stack.back());
	redo_stack.pop_back();
	last->execute_redo();
	undo_stack.push_back(move(last));
}

void Service::undo_user()
{
	if (undo_stack_user.size() == 0)
		throw std::exception{ "Undo stack is empty" };

	auto last = std::move(undo_stack_user.back());
	undo_stack_user.pop_back();
	last->execute_undo();
	this->notify();
	redo_stack_user.push_back(move(last));
}

void Service::redo_user()
{
	if (redo_stack_user.size() == 0)
		throw std::exception{ "Redo stack is empty" };
	auto last = move(redo_stack_user.back());
	redo_stack_user.pop_back();
	last->execute_redo();
	this->notify();
	undo_stack_user.push_back(move(last));
}




//-----------------------------------------------TESTS-----------------------------------------------//



void ServiceTest::add_plant_cluster__valid_input__adds_to_repository()
{
	InMemoryRepository memory_repository{};
	Repository* repository= &memory_repository;
	Service service{repository};
	service.set_file_name("test.txt");
	service.set_mylist_name("test.html");
	service.add_plant_cluster("1", "1", 9, "hsgas");
	assert(service.get_all_plant_clusters().size() == 1);

}

void ServiceTest::add_plant_cluster__invalid_input__throws_exception()
{
	InMemoryRepository memory_repository{};
	Repository* repository = &memory_repository;
	Service service{ repository };
	service.set_file_name("test.txt");
	service.set_mylist_name("test.html");
	service.add_plant_cluster("1", "1", 9, "hsgas");
	std::string error;
	try{service.add_plant_cluster("1", "w1", 93, "hsgaws");}
	catch (RepositoryException & exception)
	{
		error = exception.what();
	}
	assert(error == "Plant cluster already exists! \n");
}


void ServiceTest::delete_plant_cluster__valid_input__deletes_from_repository()
{
	InMemoryRepository memory_repository{};
	Repository* repository = &memory_repository;
	Service service{ repository };
	service.set_file_name("test.txt");
	service.set_mylist_name("test.html");
	service.add_plant_cluster("1", "1", 9, "hsgas");
	service.delete_plant_cluster("1");
	assert(service.get_all_plant_clusters().size() == 0);

}

void ServiceTest::delete_plant_cluster__invalid_input__trows_exception()
{
	InMemoryRepository memory_repository{};
	Repository* repository = &memory_repository;
	Service service{ repository };
	service.set_file_name("test.txt");
	service.set_mylist_name("test.html");
	std::string error;
	try{service.delete_plant_cluster("1");}
	catch (RepositoryException & exception)
	{
		error = exception.what();
	}
	assert(error == "Plant cluster does not exist! \n");

}

void ServiceTest::update_plant_cluster__valid_input__updates_in_repostory()
{

	InMemoryRepository memory_repository{};
	Repository* repository = &memory_repository;
	Service service{ repository };
	service.set_file_name("test.txt");
	service.set_mylist_name("test.html");
	service.add_plant_cluster("1", "1", 9, "hsgas");
	service.update_plant_cluster("1", "aggs", 72, "hgfgsa");
	std::vector<PlantCluster> array(service.get_all_plant_clusters());
	assert(array.begin()->get_age_in_months() == 72);
}

void ServiceTest::update_plant_cluster__invalid_input__trows_exception()
{
	InMemoryRepository memory_repository{};
	Repository* repository = &memory_repository;
	Service service{ repository };
	service.set_file_name("test.txt");
	service.set_mylist_name("test.html");
	std::string error;
	try{	service.update_plant_cluster("1", "526", 62, "gaghst");}
	catch (RepositoryException & exception)
	{
		error = exception.what();
	}
	assert(error == "Plant cluster does not exist! \n");
}

void ServiceTest::get_all_plant_clusters__valid_input__gets_list()
{
	InMemoryRepository memory_repository{};
	Repository* repository = &memory_repository;
	Service service{ repository };
	service.set_file_name("test.txt");
	service.set_mylist_name("test.html");
	service.add_plant_cluster("1", "1", 9, "hsgas");
	service.add_plant_cluster("21", "12", 39, "hs3gas");
	std::vector<PlantCluster> array(service.get_all_plant_clusters());
	assert(array.size() == 2);
}

void ServiceTest::initial_iterator__valid_input__initialiazes_the_iterator()
{

	InMemoryRepository memory_repository{};
	Repository* repository = &memory_repository;
	Service service{ repository };
	service.set_file_name("test.txt");
	service.set_mylist_name("test.html");
	service.add_plant_cluster("1", "1", 9, "hsgas");
	service.add_plant_cluster("21", "12", 39, "hs3gas");

	assert(service.get_current() == *(service.get_all_plant_clusters().begin()));
}

void ServiceTest::get_current__valid_input__returns_the_current_plant_cluster()
{
	InMemoryRepository memory_repository{};
	Repository* repository = &memory_repository;
	Service service{ repository };
	service.set_file_name("test.txt");
	service.set_mylist_name("test.html");
	service.add_plant_cluster("1", "1", 9, "hsgas");
	service.add_plant_cluster("21", "12", 39, "hs3gas");
	assert(service.get_current() == *service.get_all_plant_clusters().begin());
}


void ServiceTest::next__valid_input__sets_next()
{
	InMemoryRepository memory_repository{};
	Repository* repository = &memory_repository;
	Service service{ repository };
	service.set_file_name("test.txt");
	service.set_mylist_name("test.html");
	service.add_plant_cluster("1", "1", 9, "hsgas");
	service.add_plant_cluster("21", "12", 39, "hs3gas");

	service.next("", -1);
	assert(service.get_current() == *(service.get_all_plant_clusters().begin() + 1));
}

void ServiceTest::next_with_parameters__valid_input_sets_next_with_paramaters()
{
	InMemoryRepository memory_repository{};
	Repository* repository = &memory_repository;
	Service service{ repository };
	service.set_file_name("test.txt");
	service.set_mylist_name("test.html");
	service.add_plant_cluster("1", "1", 9, "hsgas");
	service.add_plant_cluster("21", "1", 39, "hs3gas");

	service.next("1", 50);
}

void ServiceTest::save_plant_cluster__valid_input__saves_inlist()
{
	InMemoryRepository memory_repository{};
	Repository* repository = &memory_repository;
	Service service{ repository };
	service.set_file_name("test.txt");
	service.set_mylist_name("test.html");
	service.add_plant_cluster("1", "1", 9, "hsgas");
	service.add_plant_cluster("21", "12", 39, "hs3gas");
	service.save_plant_cluster("1");
	assert(*service.get_user_list().begin() == service.get_current());
	service.erase_plant_cluster("1");
}

void ServiceTest::get_user_list__valid_input__gets_the_list()
{
	InMemoryRepository memory_repository{};
	Repository* repository = &memory_repository;
	Service service{ repository };
	service.set_file_name("test.txt");
	service.set_mylist_name("test.csv");
	service.add_plant_cluster("1", "1", 9, "hsgas");
	service.add_plant_cluster("21", "12", 39, "hs3gas");
	service.save_plant_cluster("21");
	service.next();
	assert(*service.get_user_list().begin() == service.get_current());
	service.erase_plant_cluster("21");
}

void ServiceTest::test_service()
{

	this->add_plant_cluster__valid_input__adds_to_repository();
	this->add_plant_cluster__invalid_input__throws_exception();
	this->delete_plant_cluster__valid_input__deletes_from_repository();
	this->delete_plant_cluster__invalid_input__trows_exception();
	this->update_plant_cluster__valid_input__updates_in_repostory();
	this->update_plant_cluster__invalid_input__trows_exception();
	this->get_all_plant_clusters__valid_input__gets_list();
	this->initial_iterator__valid_input__initialiazes_the_iterator();
	this->get_current__valid_input__returns_the_current_plant_cluster();
	this->next__valid_input__sets_next();
	this->next_with_parameters__valid_input_sets_next_with_paramaters();

	this->save_plant_cluster__valid_input__saves_inlist();
	this->get_user_list__valid_input__gets_the_list();
}
