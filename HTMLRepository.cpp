#include "HTMLRepository.h"
#include <algorithm>

std::stringstream HTMLRepository::read_file()
{
	std::stringstream file_content;
	this->file_in.open(this->file_name, std::ofstream::app);
	file_content << file_in.rdbuf();
	file_in.close();
	if (file_content.str().size() == 0)
		this->write_file(file_content);

	html_to_text(file_content);
	std::string file_content_string = file_content.str();
	file_content.str(file_content_string);

	return file_content;
}

void HTMLRepository::write_file(std::stringstream& file_content)
{

	this->file_out.open(this->file_name, std::ofstream::trunc);
	text_to_html(file_content);
	std::string file_content_string = file_content.str();
	file_content.str(file_content_string);
	this->file_out << file_content.rdbuf();
	file_out.close();
}

void HTMLRepository::html_to_text(std::stringstream& file_content)
{

	std::string file_content_string = file_content.str();

	file_content_string.erase(std::remove(file_content_string.begin(), file_content_string.end(), '\n'), file_content_string.end());

	std::string file_header = "<!DOCTYPE html><html><head><title>Plant Clusters</title></head><body><table border=\"1\"><tr><td>Coded Name</td><td>Species</td><td>Age In Months</td><td>Digitized Scan</td></tr>";
	std::string file_bottom = "</table></body></html>";

	auto header_position = file_content_string.find(file_header);
	if (header_position != std::string::npos)
		file_content_string.erase(header_position, file_header.size());

	auto bottom_position = file_content_string.find(file_bottom);
	if (bottom_position != std::string::npos)
		file_content_string.erase(bottom_position, file_bottom.size());

	std::string row_start = "<tr>";
	auto row_start_position = file_content_string.find(row_start);
	while (row_start_position != std::string::npos)
	{
		file_content_string.erase(row_start_position, row_start.size());
		row_start_position = file_content_string.find(row_start);
	}

	std::string row_end = "</td></tr>";
	auto row_end_position = file_content_string.find(row_end);
	while (row_end_position != std::string::npos)
	{
		file_content_string.replace(row_end_position, row_end.size(), "\n");
		row_end_position = file_content_string.find(row_end);
	}

	std::string cell_start = "<td>";
	auto cell_start_position = file_content_string.find(cell_start);
	while (cell_start_position != std::string::npos)
	{
		file_content_string.erase(cell_start_position, cell_start.size());
		cell_start_position = file_content_string.find(cell_start);
	}

	std::string cell_end = "</td>";
	auto cell_end_position = file_content_string.find(cell_end);
	while (cell_end_position != std::string::npos)
	{
		file_content_string.replace(cell_end_position, cell_end.size(), " ");
		cell_end_position = file_content_string.find(cell_end);
	}

	std::string link_start = "<a href=\"";
	auto link_start_position = file_content_string.find(link_start);
	while (link_start_position != std::string::npos)
	{
		file_content_string.erase(link_start_position, link_start.size());
		link_start_position = file_content_string.find(link_start);
	}

	std::string link_end = "\">Link</a>";
	auto link_end_position = file_content_string.find(link_end);
	while (link_end_position != std::string::npos)
	{
		file_content_string.erase(link_end_position, link_end.size());
		link_end_position = file_content_string.find(link_end);
	}

	file_content.str(file_content_string);

}

void HTMLRepository::text_to_html(std::stringstream& file_content)
{

	std::string file_header = "<!DOCTYPE html><html><head><title>Plant Clusters</title></head><body><table border=\"1\"><tr><td>Coded Name</td><td>Species</td><td>Age In Months</td><td>Digitized Scan</td></tr>";
	std::string file_bottom = "</table></body></html>";
	std::string file_content_string = file_header;
	std::string plant_cluster_string;

	while (getline(file_content, plant_cluster_string))
	{
		auto plant_cluster = PlantCluster(plant_cluster_string);
		file_content_string += plant_cluster.get_plant_cluster_as_html_row();
	}

	file_content_string += file_bottom;
	file_content.str(file_content_string);

}
