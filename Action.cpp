#include "Action.h"

ActionAdd::ActionAdd(PlantCluster plant_cluster, Repository* repository)
{
	this->added_plant_cluster = plant_cluster;
	this->repository = repository;
}

void ActionAdd::execute_undo()
{
	repository->delete_plant_cluster(added_plant_cluster);
}

void ActionAdd::execute_redo()
{
	repository->add_plant_cluster(added_plant_cluster);
}

ActionDelete::ActionDelete(PlantCluster plant_cluster, Repository* repository)
{
	this->deleted_plant_cluster = plant_cluster;
	this->repository = repository;
}

void ActionDelete::execute_undo()
{
	repository->add_plant_cluster(deleted_plant_cluster);
}

void ActionDelete::execute_redo()
{
	repository->delete_plant_cluster(deleted_plant_cluster);
}

ActionUpdate::ActionUpdate(PlantCluster new_plant_cluster, PlantCluster previous_plant_cluster,  Repository* repository)
{
	this->new_plant_cluster = new_plant_cluster;
	this->previous_plant_cluster = previous_plant_cluster;
	this->repository = repository;
}

void ActionUpdate::execute_undo()
{

	repository->update_plant_cluster(previous_plant_cluster);
}

void ActionUpdate::execute_redo()
{
	repository->update_plant_cluster(new_plant_cluster);
}
