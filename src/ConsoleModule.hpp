
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
};

#endif // MIDI_MAPPER_MODULE
