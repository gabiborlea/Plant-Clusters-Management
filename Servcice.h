#pragma once
#include "Repository.h"
#include "InMemoryRepository.h"
#include "CSVRepository.h"
#include "HTMLRepository.h"
#include "Validator.h"
#include <vector>
#include "Action.h"
#include <memory>
#include "Observer.h"

class Service : public Observable
{
private:
	Repository* repository;
	Repository* user_list ;
	std::string mylist_name;
	int iterator_position;
	std::vector<PlantCluster> iterated_list;
	std::vector<std::unique_ptr<Action>> undo_stack;
	std::vector<std::unique_ptr<Action>> redo_stack;
	std::vector<std::unique_ptr<Action>> undo_stack_user;
	std::vector<std::unique_ptr<Action>> redo_stack_user;

public:
	
	//constructor
	//input: repository
	Service(Repository* repository);
	Service(std::string config);
	Service(const Service& service) = delete;
	//destructor
	~Service();

	void set_file_name(std::string file_name);

	void set_mylist_name(std::string mylistfile_name);

	//this function adds to the repository a new plant cluster with the given paramters
	//input: coded_name, species, age_in_months, digitized_scan
	void add_plant_cluster(const std::string coded_name, const std::string species, const int age_in_months, const std::string digitized_scan);
	
	//this function deletes from the repository by its coded name
	//input: coded name
	void delete_plant_cluster(const std::string coded_name);

	//this function updates a plant cluster by its coded name with new parameters
	//input: coded_name, new_species, new_age_in_months, new_digitized_scan
	void update_plant_cluster(const std::string coded_name, const std::string new_species, const int new_age_in_months, const std::string new_digitized_scan);

	PlantCluster get_plant_cluster_by_coded_name(const std::string coded_name);

	//this function gets the list of all plant cluster as a dynamic array
	std::vector<PlantCluster> get_all_plant_clusters() const;

	PlantCluster get_current();

	std::vector<PlantCluster> filter_plant_clusters(std::string species, int age_in_months) const;
	std::vector<PlantCluster> filter_mylist(std::string species, int age_in_months) const;

	void open_in_app();

	void next(std::string species="", int age_in_months = -1);

	void save_plant_cluster(const std::string coded_name);

	void erase_plant_cluster(const std::string coded_name);

	std::vector<PlantCluster> get_user_list() const;
	void undo();
	void redo();
	void undo_user();
	void redo_user();
};

class ServiceTest
{
private:

	void add_plant_cluster__valid_input__adds_to_repository();
	void add_plant_cluster__invalid_input__throws_exception();
	void delete_plant_cluster__valid_input__deletes_from_repository();
	void delete_plant_cluster__invalid_input__trows_exception();
	void update_plant_cluster__valid_input__updates_in_repostory();
	void update_plant_cluster__invalid_input__trows_exception();
	void get_all_plant_clusters__valid_input__gets_list();
	void initial_iterator__valid_input__initialiazes_the_iterator();
	void get_current__valid_input__returns_the_current_plant_cluster();
	void next__valid_input__sets_next();
	void next_with_parameters__valid_input_sets_next_with_paramaters();
	void save_plant_cluster__valid_input__saves_inlist();
	void get_user_list__valid_input__gets_the_list();
public:

	void test_service();
};
