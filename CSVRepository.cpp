#include "CSVRepository.h"
#include <algorithm>

std::stringstream CSVRepository::read_file()
{
	std::stringstream file_content;
	this->file_in.open(this->file_name, std::ofstream::app);
	file_content << file_in.rdbuf();
	file_in.close();

	csv_to_text(file_content);

	return file_content;
}

void CSVRepository::write_file(std::stringstream& file_content)
{
	this->file_out.open(this->file_name, std::ofstream::trunc);
	text_to_csv(file_content);

	this->file_out << file_content.rdbuf();
	file_out.close();
}
void CSVRepository::csv_to_text(std::stringstream& file_content)
{
	std::string file_content_string = file_content.str();
	std::replace(file_content_string.begin(), file_content_string.end(), ',', ' ');
	file_content.str(file_content_string);

}

void CSVRepository::text_to_csv(std::stringstream& file_content)
{
	std::string file_content_string = file_content.str();
	std::replace(file_content_string.begin(), file_content_string.end(), ' ', ',');
	file_content.str(file_content_string);


}


