#pragma once
#include "FileRepository.h"
class CSVRepository : public FileRepository
{
public:
	std::stringstream read_file() override;
	void write_file(std::stringstream& file_content) override;
	void csv_to_text(std::stringstream& file_content);
	void text_to_csv(std::stringstream& file_content);
};