#include "PlantClusterGUI.h"
#include <QtWidgets/QApplication>
#include "PlantCluster.h"
#include "Repository.h"
#include "InMemoryRepository.h"
#include "FileRepository.h"
#include "CSVRepository.h"
#include "HTMLRepository.h"
#include "Servcice.h"
#include "Validator.h"
#include <crtdbg.h>
#include <memory>


int main(int argc, char* argv[])
{

	QApplication a(argc, argv);
	//for using in memory write 'fileLocation:inMemory' or 'mylistLocation:inMemory' in config 
	//for using file wirite 'fileLocation:filename' or 'mylistLocation:filename' in config
	std::string config_file = "config.txt";
	Service service{ config_file };
	PlantClusterGUI GUI{ service };
	GUI.show();
	return a.exec();
}
