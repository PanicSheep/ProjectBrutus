#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <string>

namespace ConfigFile
{
	extern std::map<std::string, std::string> Configurations;

	void Initialize(const std::string & filename);
}