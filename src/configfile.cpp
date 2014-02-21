#include "configfile.h"

namespace ConfigFile
{
	std::map<std::string, std::string> Configurations;

	void Initialize(const std::string & filename)
	{
		std::size_t pos;
		std::string s, key;
		std::ifstream file;
		file.open(filename);
		if(!file){
			std::cerr << "ERROR: File '" << filename << "' could not be opened!" << std::endl;
			return;
		}

		while (std::getline(file, s)){
			pos = s.find(" = ");
			key = s.substr(0, pos);
			s.erase(0, pos + 3);
			Configurations[key] = s;
		}

		file.close();
	}
}