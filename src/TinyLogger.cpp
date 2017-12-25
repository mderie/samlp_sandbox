
#include "TinyLogger.hpp"
//#include "MidiMapperPlugin.hpp" // Care circular ref !
#include "ConfigurationFiles.hpp"

#include <ctime>

#include "rack.hpp"
using namespace rack;

extern Plugin *thisPlugin;
extern ConfigurationFile *cf;

void logf(const char *s)
{
	if (cf and (cf->keyValue("Misc", "Debug") == "0"))
	{
		return;
	}

	time_t rawtime;
	struct tm* timeinfo;
	char timestamp[32]; // Sure there is more C++ way to do this...

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(timestamp, sizeof(timestamp), "%d/%m/%Y %H:%M:%S", timeinfo);

	std::string fullFilename = assetPlugin(thisPlugin, "MidiMapper.log"); // Care : we need the plugin instance here...
	FILE *fLog = fopen(fullFilename.c_str(), "a");
	fprintf(fLog, "%s => %s\n", timestamp, s);
	fclose(fLog);
}
