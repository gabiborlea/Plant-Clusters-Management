#pragma once
#include "PlantCluster.h"
#include <vector>
#include <exception>

class RepositoryException : public std::exception {
private:
	const char* exception;
public:

	RepositoryException(const char* exception) { this->exception = exception; }
	~RepositoryException() {}
	const char* what() const throw() { return this->exception; }
};


class Repository
{
public:
	virtual void set_file_name(std::string file_name) = 0;

	virtual int find_plant_cluster(const PlantCluster& plant_cluster) = 0;

	virtual std::vector<PlantCluster> get_plant_clusters() = 0;

	virtual void add_plant_cluster(const PlantCluster& plant_cluster) = 0;

	virtual void delete_plant_cluster(const PlantCluster& plant_cluster) = 0;

	virtual void update_plant_cluster(const PlantCluster& new_plant_cluster) = 0;

	virtual PlantCluster get_plant_cluster(const PlantCluster& plant_cluster) = 0;

	virtual ~Repository() {}
};