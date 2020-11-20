#pragma once
#include <string>

class PlantCluster
{
private:
	std::string coded_name;
	std::string species;
	int age_in_months;
	std::string digitized_scan;

public:
	//default constructor
	PlantCluster();

	//constructor with parameters
	PlantCluster(const std::string coded_name, const std::string species, const int age_in_months,const std::string digitized_scan);

	//copy_constrictor
	PlantCluster(const PlantCluster& plant_cluster);

	PlantCluster(std::string plant_cluster_string, char delimitator = ' ');

	//destructor
	~PlantCluster();

	std::string get_coded_name() const { return this->coded_name; }
	std::string get_species() const { return this->species; }
	int get_age_in_months() const { return this->age_in_months; }
	std::string get_digitized_scan() const { return this->digitized_scan; }

	//overloads the equals operator
	//returns true if their coded name are equal; false otherwise
	bool operator==(PlantCluster another_plant_cluster);

	std::string get_plant_cluster_as_string() const;


	std::string get_plant_cluster_as_html_row() const;
};

class PlantClusterTest
{
private:
	void default_constructor__valid_input__creates_new_empty_object();
	void constructor__valid_input__creates_new_object();
	void copy_constructor__valid_input__creates_copy_object();
	void operator_eqauls__valid_input__returns_true();
	void operator_eqauls__invalid_input__returns_false();

public:
	void test_plant_cluster();
};
