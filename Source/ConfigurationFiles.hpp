
#ifndef CONFIGURATION_FILES
#define CONFIGURATION_FILES

#include <string>
#include <map> // We could also use unordered map here...
#include <vector>

// Base class for all configuration files...
//TODO: Implement default value and section support
class ConfigurationFile
{
private:
  std::map<std::string, std::map<std::string, std::string> > m_contents; // The "> > " means I'm old school :)
public:
	ConfigurationFile(const std::string &fullFilename);
	//Not needed ? virtual ~ConfigurationFile();
	~ConfigurationFile();
	std::string keyValue(const std::string &section, const std::string &key);
	std::vector<std::string> keyNames(const std::string &section);
	std::vector<std::string> sections();

	//TODO: Implement this ?
	//bool sectionExists(std::string& section);
	//bool keyExists(std::string& section; std::string& key);
	bool valueExists(const std::string& section, const std::string& value);
};

#endif // CONFIGURATION_FILES
