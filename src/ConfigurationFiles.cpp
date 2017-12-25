
#include <iostream>
#include <fstream>

#include "ConfigurationFiles.hpp"

ConfigurationFile::ConfigurationFile(const std::string& fullFilename)
{
	std::ifstream ifs;
	std::string line;
	size_t pos;
	std::string section;

	//std::cout << "fullFilename = " << fullFilename << std::endl;

	ifs.open(fullFilename);
	while(!ifs.eof())
	{
		std::getline(ifs, line);
		//std::cout << "line = " << line << std::endl;
		if (line[0] == ';')
		{
			//std::cout << "Found comment = " << line << std::endl;
			continue; // Just ignore comment :)
		}

		if (line.size() == 0)
		{
			//std::cout << "Found empty line !" << line << std::endl;
			continue; // Just ignore empty line
		}

		if ((line.front() == '[') and (line.back() == ']'))
		{
			section = line.substr(1, line.size() - 2); // We also use cbegin() and cend() but why bother !
			//std::cout << "Found section = " << section << std::endl;
		}
		else if ((pos = line.find('=', 0)) != std::string::npos)
		{
			std::cout << "section = " << section << "; key = '" << line.substr(0, pos) << "'; value = '" << line.substr(pos + 1, line.size() - pos + 1) << "'" << std::endl;
			m_contents[section][line.substr(0, pos)] = line.substr(pos + 1, line.size() - pos + 1);
		}
		else
		{
		  //TODO: Make use of the log here ?
			//std::cout << "Found bad line in filename = " << fullFilename << " value = " << line << std::endl;
			/*
			We can't log yet ! Can we ?
			logThis("Found error in the configuration file !", Target::misc);
			logThis("Filename = ...", Target::misc);
			logThis(filename.c_str(), Target::misc);
			logThis("Line = ...", Target::misc);
			logThis(line.c_str(), Target::misc);
			*/
		}
	}
	ifs.close();
}

ConfigurationFile::~ConfigurationFile()
{
	m_contents.clear(); // Shall I propagate the clear ?
}

std::vector<std::string> ConfigurationFile::sections()
{
	std::vector<std::string> result;

	for(auto const& kvp : m_contents)
	{
    result.push_back(kvp.first);
	}
	return result;
}

std::vector<std::string> ConfigurationFile::keyNames(const std::string &section)
{
  std::vector<std::string> result;
  for(auto const &kvp : m_contents[section])
  {
    result.push_back(kvp.first);
	}

	return result;
}

std::string ConfigurationFile::keyValue(const std::string &section, const std::string &key)
{
	return m_contents[section][key];
}

bool ConfigurationFile::valueExists(const std::string& section, const std::string& value)
{
  for(auto const &kvp : m_contents[section])
  {
    if (kvp.second == value)
    {
			return true;
    }
	}

	return false;
}

void ConfigurationFile::insert(const std::string& section, const std::string& key, const std::string& value)
{
	m_contents[section][key] = value;
}

void ConfigurationFile::remove(const std::string& section, const std::string& key)
{
  m_contents[section].erase(key);
}

void ConfigurationFile::save(const std::string &fullFilename)
{
  std::ofstream ofs(fullFilename);

 	for(auto const& section : m_contents)
	{
		ofs << "[" << section.first << "]" << std::endl;
		for(auto const &key : section.second)
		{
    	ofs << key.first << " = " << key.second << std::endl;
    }
	}

	ofs.close();
}
