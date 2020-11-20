#include "FileRepository.h"
#include <assert.h>
void FileRepository::set_file_name(std::string file_name)
{
	this->file_name = file_name;
	read_file();
}
std::stringstream FileRepository::read_file()
{
	std::stringstream file_content;
	this->file_in.open(this->file_name, std::ofstream::app);
	file_content << file_in.rdbuf();
	file_in.close();

	return file_content;
}

void FileRepository::write_file(std::stringstream& file_content)
{

	this->file_out.open(this->file_name, std::ofstream::trunc);
	this->file_out << file_content.rdbuf();
	file_out.close();
}

int FileRepository::find_plant_cluster(const PlantCluster& plant_cluster)
{
	auto file_content = this->read_file();
	int position = 0;
	std::string plant_cluster_string = plant_cluster.get_plant_cluster_as_string();
	std::string line;
	while (getline(file_content, line))
	{
		if (PlantCluster(line) == PlantCluster(plant_cluster_string))
			return position;
		position++;
	}
	return -1;
}

std::vector<PlantCluster> FileRepository::get_plant_clusters()
{
	auto file_content = this->read_file();
	std::string plant_cluster_string;
	std::vector<PlantCluster> plant_clusters;
	while (std::getline(file_content, plant_cluster_string))
	{
		plant_clusters.push_back(PlantCluster(plant_cluster_string));
	}
	return plant_clusters;
}

void FileRepository::add_plant_cluster(const PlantCluster& plant_cluster)
{
	if (find_plant_cluster(plant_cluster) != -1)
		throw RepositoryException("Plant cluster already exists! \n");

	auto file_content = this->read_file();
	std::string string_content = file_content.str();

	string_content += plant_cluster.get_plant_cluster_as_string() + "\n";
	file_content.str(string_content);
	write_file(file_content);
}

void FileRepository::delete_plant_cluster(const PlantCluster& plant_cluster)
{

	int position = find_plant_cluster(plant_cluster);
	if (position == -1)
		throw RepositoryException("Plant cluster does not exist! \n");

	std::string plant_cluster_string = this->get_plant_cluster(plant_cluster).get_plant_cluster_as_string();

	auto file_content = this->read_file();
	std::string string_content = file_content.str();


	auto substring_position = string_content.find(plant_cluster_string);
	auto finish= plant_cluster_string.size() + 1;

	string_content.erase(substring_position, finish);

	file_content.str(string_content);
	write_file(file_content);
}

void FileRepository::update_plant_cluster(const PlantCluster& new_plant_cluster)
{

	int position = find_plant_cluster(new_plant_cluster);
	if (position == -1)
		throw RepositoryException("Plant cluster does not exist! \n");

	PlantCluster deleted_plant_cluster = this->get_plant_cluster(new_plant_cluster);
	std::string deleted_plant_cluster_string = deleted_plant_cluster.get_plant_cluster_as_string();

	auto file_content = this->read_file();
	std::string string_content = file_content.str();

	auto substring_position = string_content.find(deleted_plant_cluster_string);
	string_content.replace(substring_position, deleted_plant_cluster_string.size(), new_plant_cluster.get_plant_cluster_as_string());
	
	file_content.str(string_content);
	write_file(file_content);

}

PlantCluster FileRepository::get_plant_cluster(const PlantCluster& plant_cluster)
{
	auto file_content = this->read_file();
	std::string plant_cluster_string = plant_cluster.get_plant_cluster_as_string();
	std::string line;
	while (getline(file_content, line))
	{
		if (PlantCluster(line) == PlantCluster(plant_cluster_string))
			return PlantCluster(line);
	}
	throw RepositoryException("Plant cluster does not exist! \n");
}

void FileRepositoryTest::find_plant_cluster__valid_input__gets_its_position()
{

	std::ofstream file;
	file.open("text.txt");
	file.clear();
	FileRepository repository{"text.txt"};
	PlantCluster plant_cluster{ "1", "1", 1, "1" };
	repository.add_plant_cluster(plant_cluster);
	assert(repository.find_plant_cluster(plant_cluster) != -1);
}

void FileRepositoryTest::find_plant_cluster__invalid_input__returns_minus_1()
{
	std::ofstream file;
	file.open("text.txt");
	file.clear();
	FileRepository repository{ "text.txt" };
	PlantCluster plant_cluster{ "1", "1", 1, "1" };
	assert(repository.find_plant_cluster(plant_cluster) == -1);
}

void FileRepositoryTest::add_plant_cluster__valid_input__adds_new_plant_cluster()
{
	std::ofstream file;
	file.open("text.txt");
	file.clear();
	FileRepository repository{ "text.txt" };
	PlantCluster plant_cluster{ "1", "1", 1, "1" };
	repository.add_plant_cluster(plant_cluster);
	assert(repository.find_plant_cluster(plant_cluster) == 0);
}

void FileRepositoryTest::add_plant_cluster__invalid_input__throws_error()
{

	std::ofstream file;
	file.open("text.txt");
	file.clear();
	FileRepository repository{ "text.txt" };
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

void FileRepositoryTest::delete_plant_cluster__valid_input__deletes_plant_cluster()
{
	std::ofstream file;
	file.open("text.txt");
	file.clear();
	FileRepository repository{ "text.txt" };
	PlantCluster plant_cluster{ "1", "1", 1, "1" };
	repository.add_plant_cluster(plant_cluster);
	repository.delete_plant_cluster(plant_cluster);
	assert(repository.find_plant_cluster(plant_cluster) == -1);
}

void FileRepositoryTest::delete_plant_cluster__invalid_input__throws_exception()
{
	std::ofstream file;
	file.open("text.txt");
	file.clear();
	FileRepository repository{ "text.txt" };
	PlantCluster plant_cluster{ "1", "1", 1, "1" };
	std::string error;
	try{repository.delete_plant_cluster(plant_cluster);}
	catch (RepositoryException & exception)
	{
		error = exception.what();
	}
	assert(error == "Plant cluster does not exist! \n");
}

void FileRepositoryTest::update_plant_cluster__valid_input__updates_plant_cluster()
{
	std::ofstream file;
	file.open("text.txt");
	file.clear();
	FileRepository repository{ "text.txt" };
	PlantCluster plant_cluster{ "1", "1", 1, "1" };
	repository.add_plant_cluster(plant_cluster);
	PlantCluster new_plant_cluster{ "1", "ahgsa", 763, "hsgas" };
	repository.update_plant_cluster(new_plant_cluster);

	auto array = repository.get_plant_clusters();
	auto the_plant_cluster{ *array.begin() };
	assert(the_plant_cluster.get_age_in_months() == 763);
}

void FileRepositoryTest::update_plant_cluster__invalid_input__throws_exception()
{
	std::ofstream file;
	file.open("text.txt");
	file.clear();
	FileRepository repository{ "text.txt" };
	PlantCluster plant_cluster{ "1", "1", 1, "1" };
	std::string error;
	try{repository.update_plant_cluster(plant_cluster);}
	catch (RepositoryException & exception)
	{
		error = exception.what();
	}
	assert(error == "Plant cluster does not exist! \n");
}

void FileRepositoryTest::get_plant_clusters__valid_input__gets_the_list()
{
	std::ofstream file;
	file.open("text.txt");
	file.clear();
	PlantCluster plant_cluster{ "1", "1", 1, "1" };
	FileRepository repository{ "text.txt" };
	repository.add_plant_cluster(plant_cluster);
	std::vector<PlantCluster> array{ repository.get_plant_clusters() };
	assert(array.size() == 1);
}

void FileRepositoryTest::get_plant_cluster__valid_input__gets_plant_cluster()
{
	std::ofstream file;
	file.open("text.txt");
	file.clear();
	PlantCluster plant_cluster{ "1", "1", 1, "1" };
	FileRepository repository{ "text.txt" };
	repository.add_plant_cluster(plant_cluster);
	assert(repository.get_plant_cluster(PlantCluster("1", "0", 0, "0")) == plant_cluster);
}

void FileRepositoryTest::get_plant_cluster__invalid_input__throws_error()
{
	std::ofstream file;
	file.open("text.txt");
	file.clear();
	PlantCluster plant_cluster{ "1", "1", 1, "1" };
	FileRepository repository{ "text.txt" };
	repository.add_plant_cluster(plant_cluster);
	std::string error;
	try { repository.get_plant_cluster(PlantCluster("2", "0", 0, "0")); }
	catch (RepositoryException & exception)
	{
		error = exception.what();
	}
	assert(error == "Plant cluster does not exist! \n");
}

void FileRepositoryTest::set_file_name__valid_input__sets_file_name()
{
	std::ofstream file;
	file.open("text.txt");
	file.clear();
	PlantCluster plant_cluster{ "1", "1", 1, "1" };
	FileRepository repository{ "text.txt" };
	repository.set_file_name("ceva.txt");

}

void FileRepositoryTest::test_repository()
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
	set_file_name__valid_input__sets_file_name();

}

