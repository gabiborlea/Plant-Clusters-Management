#include "PlantCluster.h"
#include <assert.h>
#include <sstream>

PlantCluster::PlantCluster() : coded_name(""), species(""), age_in_months(0), digitized_scan("") {}

PlantCluster::PlantCluster(const std::string coded_name, const std::string species, const int age_in_months, const std::string digitized_scan)
{
	this->coded_name = coded_name;
	this->species = species;
	this->age_in_months = age_in_months;
	this->digitized_scan = digitized_scan;
}

PlantCluster::PlantCluster(const PlantCluster& plant_cluster)
{
	this->coded_name = plant_cluster.coded_name;
	this->species = plant_cluster.species;
	this->age_in_months = plant_cluster.age_in_months;
	this->digitized_scan = plant_cluster.digitized_scan;
}

PlantCluster::PlantCluster(std::string plant_cluster_string, char delimitator)
{
	std::istringstream plant_cluster_stream(plant_cluster_string);
	std::string token;

	std::getline(plant_cluster_stream, token, delimitator);
	this->coded_name = token;

	std::getline(plant_cluster_stream, token, delimitator);
	this->species = token;

	std::getline(plant_cluster_stream, token, delimitator);
	this->age_in_months = std::stoi(token);

	std::getline(plant_cluster_stream, token, delimitator);
	this->digitized_scan = token;
}

PlantCluster::~PlantCluster() {}

bool PlantCluster::operator==(PlantCluster another_plant_cluster)
{
	return this->coded_name == another_plant_cluster.coded_name;
}

std::string PlantCluster::get_plant_cluster_as_string() const
{
	return this->get_coded_name() + " " + this->get_species() + " " + std::to_string(this->get_age_in_months()) + " " + this->get_digitized_scan();
}

std::string PlantCluster::get_plant_cluster_as_html_row() const
{
	return "<tr><td>" + this->get_coded_name() + "</td><td>" + this->get_species() + "</td><td>" + std::to_string(this->get_age_in_months()) + "</td><td><a href=\"" + this->get_digitized_scan() + "\">Link</a></td></tr>";
}




//-----------------------------------------------TESTS-----------------------------------------------//

void PlantClusterTest::default_constructor__valid_input__creates_new_empty_object()
{
	auto plant_cluster = PlantCluster();
	assert(plant_cluster.get_coded_name() == "" && plant_cluster.get_species() == "" && plant_cluster.get_age_in_months() == 0 && plant_cluster.get_digitized_scan() == "");
}


void PlantClusterTest::constructor__valid_input__creates_new_object()
{
	auto plant_cluster = PlantCluster( "452F", "tshd", 82, "ahsgsgd" );
	
}

void PlantClusterTest::copy_constructor__valid_input__creates_copy_object()
{
	auto plant_cluster = PlantCluster("452F", "tshd", 82, "ahsgsgd");
	auto copy_plant_cluster = PlantCluster(plant_cluster);
	assert(copy_plant_cluster.get_coded_name() == "452F" && copy_plant_cluster.get_species() == "tshd" && copy_plant_cluster.get_age_in_months() == 82 && copy_plant_cluster.get_digitized_scan() == "ahsgsgd");
}

void PlantClusterTest::operator_eqauls__valid_input__returns_true()
{
	auto plant_cluster = PlantCluster("452F", "tshd", 82, "ahsgsgd");
	auto another_plant_cluster = PlantCluster("452F", "td", 42, "agd");
	assert(plant_cluster == another_plant_cluster);
}

void PlantClusterTest::operator_eqauls__invalid_input__returns_false()
{
	auto plant_cluster = PlantCluster("452F", "tshd", 82, "ahsgsgd");
	auto another_plant_cluster = PlantCluster("4d52F", "td", 42, "agd");
	assert(!(plant_cluster == another_plant_cluster));
}



void PlantClusterTest::test_plant_cluster()
{
	this->default_constructor__valid_input__creates_new_empty_object();
	this->constructor__valid_input__creates_new_object();
	this->copy_constructor__valid_input__creates_copy_object();
	this->operator_eqauls__valid_input__returns_true();
	this->operator_eqauls__invalid_input__returns_false();
}


