#pragma once
#include "FileRepository.h"
class HTMLRepository : public FileRepository
{
public:
	std::stringstream read_file() override;
	void write_file(std::stringstream& file_content) override;
	void html_to_text(std::stringstream& file_content);
	void text_to_html(std::stringstream& file_content);
};