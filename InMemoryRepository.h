#pragma once
#include "PlantCluster.h"

#include <vector>
#include "Repository.h"



class InMemoryRepository : public Repository
{
private:
	std::vector<PlantCluster> plant_cluster_list;

public:

	//default constructer
	InMemoryRepository() { this->plant_cluster_list = std::vector<PlantCluster>();  };

	int find_plant_cluster(const PlantCluster& plant_cluster) override;

	//returns the list of plant_clusters
	std::vector<PlantCluster>get_plant_clusters() override;

	//adds a new plant cluster to the repository
	//throws exception if the plant cluster already exists
	void add_plant_cluster(const PlantCluster& plant_cluster) override;

	//deletes a plant cluster from the repository
	//throws exception if the plant cluster does not exist
	void delete_plant_cluster(const PlantCluster& plant_cluster) override;

	//updates a plant_cluster with a new one
	//throws exception if the plant cluster does not exist
	void update_plant_cluster(const PlantCluster& new_plant_cluster) override;

	PlantCluster get_plant_cluster(const PlantCluster& plant_cluster) override;

	void set_file_name(std::string file_name) {};
	
};


class InMemoryRepositoryTest
{
private:
	void find_plant_cluster__valid_input__gets_its_position();
	void find_plant_cluster__invalid_input__returns_minus_1();
	void add_plant_cluster__valid_input__adds_new_plant_cluster();
	void add_plant_cluster__invalid_input__throws_error();
	void delete_plant_cluster__valid_input__deletes_plant_cluster();
	void delete_plant_cluster__invalid_input__throws_exception();
	void update_plant_cluster__valid_input__updates_plant_cluster();
	void update_plant_cluster__invalid_input__throws_exception();
	void get_plant_clusters__valid_input__gets_the_list();
	void get_plant_cluster__valid_input__gets_plant_cluster();
	void get_plant_cluster__invalid_input__throws_error();

public:
	void test_repository();
};

