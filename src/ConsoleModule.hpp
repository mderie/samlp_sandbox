
#ifndef CONSOLE_MODULE
#define CONSOLE_MODULE

#include "rack.hpp"

struct ConsoleModule : rack::Module
{
	enum ParamIds
	{
		NUM_PARAMS
	};

	enum InputIds
	{
		NUM_INPUTS
	};

	enum OutputIds
	{
		NUM_OUTPUTS
	};

	enum LightIds
	{
		NUM_LIGHTS
	};

	void step() override;

	enum EventKind = (ekAddCable, ekAddModule, ekRemoveCable, ekRemoveCable);

	void feed(const std::string event, EventKind ek);
};

#endif // CONSOLE_MODULE
