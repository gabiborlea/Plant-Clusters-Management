#pragma once
#include "PlantCluster.h"


#include <exception>

class ValidationException : public std::exception {
private:
	const char* exception;
public:

	ValidationException(const char* exception) { this->exception = exception; }
	~ValidationException() {}
	const char* what() const throw() { return this->exception; }
};

class Validator
{
private:
	PlantCluster plant_cluster;
public:
	Validator(PlantCluster plant_cluster) { this->plant_cluster = plant_cluster; }
	void validation();
	
};

class ValidatorTest
{
private:
	void validate_plant_cluster__valid_input__does_nothing();
	void validate_plant_cluster__invalid_input__throws_exception_for_negative_number();
	void validate_plant_cluster__invalid_input__throws_exception_for_too_long_coded_name();
	void validate_plant_cluster__invalid_input__throws_exception_for_too_long_digitized_scan();

public:
	void test_all_validator();
};