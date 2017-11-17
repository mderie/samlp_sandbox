
#include "rack.hpp"

// Care : modified but finally not necessary
//#include "engine.hpp"

// Exceptionally, JUCE will be replaced here by a tiny lib...
//#include "../JuceLibraryCode/JuceHeader.h"
#include "RtMidi.h"

// Little helper .ini file reader
#include "ConfigurationFiles.hpp"

using namespace rack;

struct OnOffWidget : ModuleWidget
{
	OnOffWidget();
	~OnOffWidget();
};

struct OnOff : Module
{
	enum ParamIds
	{
		SWITCH_PARAM,
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

	OnOff();
	~OnOff();

	RtMidiIn* midiIn = 0;
	float LEDs[3] = {};
	void step();

	unsigned int dumpMidi(const std::string& nameLike);
	void dumpRack();
	void dumpConfig();
	void dumpParam(QuantityWidget* w);
	void dumpParameters();
	void startListen(unsigned int index);
	void stopListen();
};

/*
class MIC : public juce::MidiInputCallback
{
public:
	void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
private:
};
*/

//const std::string midiInputDeviceName = "BerhingerBcr2000"
//const std::string midiInputDeviceName = "NovationLaunchpad";

// Source = http://www.cplusplus.com/forum/general/33669/
template <typename T, unsigned size>
inline unsigned int sizeOfArray(const T(&)[size]) { return size; }

//TODO: use a real configuration file !
// Care replace \u that stands for Unicode with \? for one byt only
// All strings must be preceded by midi three bytes code
// TBC : the input valus is the button code for the first two bytes then the current value for the third
// TBD : the output values are all in the 0-100 intervall ?
//const std::string rotaryButtons[] =
//{
//"\u0060LFO.FREQ",
//"\u0060VCA.MAIN"
//};
