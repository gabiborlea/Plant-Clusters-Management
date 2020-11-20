#pragma once
#include "PlantCluster.h"
#include "Repository.h"
class Action
{
public:
	virtual void execute_undo() = 0;
	virtual void execute_redo() = 0;
	virtual ~Action(){}
};

class ActionAdd : public Action
{
private:
	PlantCluster added_plant_cluster;
	Repository* repository;

public:
	ActionAdd(PlantCluster plant_cluster, Repository* repository);
	void execute_undo() override;
	void execute_redo() override;

};

class ActionDelete : public Action
{
private:
	
	PlantCluster deleted_plant_cluster;
	Repository* repository;

public:
	ActionDelete(PlantCluster plant_cluster, Repository* repository);
	void execute_undo() override;
	void execute_redo() override;

};

class ActionUpdate : public Action
{
private:
	PlantCluster new_plant_cluster;
	PlantCluster previous_plant_cluster;
	Repository* repository;
public:
	ActionUpdate(PlantCluster new_plant_cluster, PlantCluster previous_plant_cluster, Repository* repository);
	void execute_undo() override;
	void execute_redo() override;
};