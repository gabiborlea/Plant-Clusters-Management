#include "InMemoryRepository.h"
#include <assert.h>

int InMemoryRepository::find_plant_cluster(const PlantCluster& plant_cluster)
{
	auto found_iterator = std::find(plant_cluster_list.begin(), plant_cluster_list.end(), plant_cluster);
	if (found_iterator == plant_cluster_list.end())
		return -1;

	return found_iterator - plant_cluster_list.begin();
}

std::vector<PlantCluster> InMemoryRepository::get_plant_clusters()
{
	return plant_cluster_list;
}

void InMemoryRepository::add_plant_cluster(const PlantCluster& plant_cluster)
{
	if (find_plant_cluster(plant_cluster) != -1)
		throw RepositoryException("Plant cluster already exists! \n");

	plant_cluster_list.push_back(plant_cluster);
}

void InMemoryRepository::delete_plant_cluster(const PlantCluster& plant_cluster)
{
	int position = find_plant_cluster(plant_cluster);
	if(position == -1)
		throw RepositoryException("Plant cluster does not exist! \n");

	plant_cluster_list.erase(position + plant_cluster_list.begin());
}

void InMemoryRepository::update_plant_cluster(const PlantCluster& new_plant_cluster)
{
	int position = find_plant_cluster(new_plant_cluster);
	if (position == -1)
		throw RepositoryException("Plant cluster does not exist! \n");

	plant_cluster_list.erase(position + plant_cluster_list.begin());
	plant_cluster_list.insert(position + plant_cluster_list.begin(), new_plant_cluster);
}

PlantCluster InMemoryRepository::get_plant_cluster(const PlantCluster& plant_cluster)
{
	int position = find_plant_cluster(plant_cluster);
	if (position == -1)
		throw RepositoryException("Plant cluster does not exist! \n");
	return *(plant_cluster_list.begin() + position);
}

void InMemoryRepositoryTest::find_plant_cluster__valid_input__gets_its_position()
{
	InMemoryRepository repository{};
	PlantCluster plant_cluster{ "1", "1", 1, "1" };
	repository.add_plant_cluster(plant_cluster);
	assert(repository.find_plant_cluster(plant_cluster) != -1);
}

void InMemoryRepositoryTest::find_plant_cluster__invalid_input__returns_minus_1()
{
	InMemoryRepository repository{};
	PlantCluster plant_cluster{ "1", "1", 1, "1" };
	assert(repository.find_plant_cluster(plant_cluster) == -1);
}

void InMemoryRepositoryTest::add_plant_cluster__valid_input__adds_new_plant_cluster()
{
	InMemoryRepository repository{};
	PlantCluster plant_cluster{ "1", "1", 1, "1" };
	repository.add_plant_cluster(plant_cluster);
	assert(repository.find_plant_cluster(plant_cluster) == 0);
}

void InMemoryRepositoryTest::add_plant_cluster__invalid_input__throws_error()
{
	InMemoryRepository repository{};
	PlantCluster plant_cluster{ "1", "1", 1, "1" };
	repository.add_plant_cluster(plant_cluster);
	std::string error;
	try{repository.add_plant_cluster(plant_cluster);}
	catch (RepositoryException & exception)
	{
		error = exception.what();
	}
	assert(error == "Plant cluster already exists! \n");
}

void InMemoryRepositoryTest::delete_plant_cluster__valid_input__deletes_plant_cluster()
{
	InMemoryRepository repository{};
	PlantCluster plant_cluster{ "1", "1", 1, "1" };
	repository.add_plant_cluster(plant_cluster);
	repository.delete_plant_cluster(plant_cluster);
	assert(repository.find_plant_cluster(plant_cluster) == -1);
}

void InMemoryRepositoryTest::delete_plant_cluster__invalid_input__throws_exception()
{
	InMemoryRepository repository{};
	PlantCluster plant_cluster{ "1", "1", 1, "1" };
	std::string error;
	try{repository.delete_plant_cluster(plant_cluster);}
	catch (RepositoryException & exception)
	{
		error = exception.what();
	}
	assert(error == "Plant cluster does not exist! \n");
}

void InMemoryRepositoryTest::update_plant_cluster__valid_input__updates_plant_cluster()
{
	InMemoryRepository repository{};
	PlantCluster plant_cluster{ "1", "1", 1, "1" };
	repository.add_plant_cluster(plant_cluster);
	PlantCluster new_plant_cluster{ "1", "ahgsa", 763, "hsgas" };
	repository.update_plant_cluster(new_plant_cluster);
	auto array = repository.get_plant_clusters();
	auto the_plant_cluster{ *array.begin() };
	assert(the_plant_cluster.get_age_in_months() == 763);
}

void InMemoryRepositoryTest::update_plant_cluster__invalid_input__throws_exception()
{
	InMemoryRepository repository{};
	PlantCluster plant_cluster{ "1", "1", 1, "1" };
	std::string error;
	try{ repository.update_plant_cluster(plant_cluster); }
	catch (RepositoryException & exception)
	{
		error = exception.what();
	}
	assert(error == "Plant cluster does not exist! \n");
}

void InMemoryRepositoryTest::get_plant_clusters__valid_input__gets_the_list()
{
	InMemoryRepository repository{};
	PlantCluster plant_cluster{ "1", "1", 1, "1" };
	repository.set_file_name("ceva");
	repository.add_plant_cluster(plant_cluster);
	std::vector<PlantCluster> array{ repository.get_plant_clusters() };
	assert(array.size() == 1);
}

void InMemoryRepositoryTest::get_plant_cluster__valid_input__gets_plant_cluster()
{
	InMemoryRepository repository{};
	PlantCluster plant_cluster{ "1", "1", 1, "1" };
	repository.add_plant_cluster(plant_cluster);
	assert(repository.get_plant_cluster(PlantCluster("1", "0", 0, "0")) == plant_cluster );
}

void InMemoryRepositoryTest::get_plant_cluster__invalid_input__throws_error()
{
	InMemoryRepository repository{};
	PlantCluster plant_cluster{ "1", "1", 1, "1" };
	repository.add_plant_cluster(plant_cluster);
	std::string error;
	try{repository.get_plant_cluster(PlantCluster("2", "0", 0, "0"));}
	catch (RepositoryException & exception)
	{
		error = exception.what();
	}
	assert(error == "Plant cluster does not exist! \n");
}


void InMemoryRepositoryTest::test_repository()
{
	find_plant_cluster__valid_input__gets_its_position();
	find_plant_cluster__invalid_input__returns_minus_1();
	add_plant_cluster__valid_input__adds_new_plant_cluster();
	add_plant_cluster__invalid_input__throws_error();
	delete_plant_cluster__valid_input__deletes_plant_cluster();
	delete_plant_cluster__invalid_input__throws_exception();
	update_plant_cluster__valid_input__updates_plant_cluster();
	update_plant_cluster__invalid_input__throws_exception();
	get_plant_clusters__valid_input__gets_the_list();
	get_plant_cluster__valid_input__gets_plant_cluster();
	get_plant_cluster__invalid_input__throws_error();
	
}
