#include "Validator.h"
#include <assert.h>

void Validator::validation()
{
	if (this->plant_cluster.get_age_in_months() < 0)
		throw ValidationException("Age in months needs to be a positive integer!\n");

	if(this->plant_cluster.get_coded_name().size() <= 0)
		throw ValidationException("Coded name needs to have at least a character!\n");

	if (this->plant_cluster.get_digitized_scan().size() <= 0)
		throw ValidationException("Digitized needs to have at least a character!\n");

	if (this->plant_cluster.get_species().size() <= 0)
		throw ValidationException("Species needs to have at least a character!\n");
	std::size_t found = plant_cluster.get_coded_name().find(" ");
	if (found != std::string::npos)
		throw ValidationException("Do not use spaces in coded name!\n");

	found = plant_cluster.get_digitized_scan().find(" ");
	if (found != std::string::npos)
		throw ValidationException("Do not use spaces in digitized scan!\n");

	found = plant_cluster.get_species().find(" ");
	if (found != std::string::npos)
		throw ValidationException("Do not use spaces in species!\n");

}

void ValidatorTest::validate_plant_cluster__valid_input__does_nothing()
{
	auto plant_cluster = PlantCluster("452F", "tshd", 82, "ahsgsgd");
	Validator plant_cluster_validator{ plant_cluster };
	plant_cluster_validator.validation();
}

void ValidatorTest::validate_plant_cluster__invalid_input__throws_exception_for_negative_number()
{
	auto plant_cluster = PlantCluster("452F", "tshd", -82, "ahsgsgd");
	Validator plant_cluster_validator{ plant_cluster };
	std::string error;
	try { plant_cluster_validator.validation(); }
	catch (ValidationException & exception)
	{
		error = exception.what();
	}

	assert(error == "Age in months needs to be a positive integer!\n");
	
}

void ValidatorTest::validate_plant_cluster__invalid_input__throws_exception_for_too_long_coded_name()
{
	auto plant_cluster = PlantCluster("452Fgahgshgajahgsbags", "tshd", 82, "ahsgsgd");
	Validator plant_cluster_validator{ plant_cluster };
	std::string error;
	try { plant_cluster_validator.validation(); }
	catch (ValidationException & exception)
	{
		error = exception.what();
	}

	assert(error == "Coded name's size needs to be less than 10!\n");
}

void ValidatorTest::validate_plant_cluster__invalid_input__throws_exception_for_too_long_digitized_scan()
{
	auto plant_cluster = PlantCluster("452F", "tshd", 82, "ahsgsgdasuhajsnaundjsanufjndjhsbjsnudnausuabnsabshbahsbahbdhabx");
	Validator plant_cluster_validator{ plant_cluster };
	std::string error;
	try { plant_cluster_validator.validation(); }
	catch (ValidationException & exception)
	{
		error = exception.what();
	}

	assert(error == "Digitized scan's size needs to be less than 20!\n");
}

void ValidatorTest::test_all_validator()
{
	this->validate_plant_cluster__valid_input__does_nothing();
	this->validate_plant_cluster__invalid_input__throws_exception_for_negative_number();
	this->validate_plant_cluster__invalid_input__throws_exception_for_too_long_coded_name();
	this->validate_plant_cluster__invalid_input__throws_exception_for_too_long_digitized_scan();
}
