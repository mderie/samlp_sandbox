
#include "rack.hpp"
#include "engine.hpp" // Care : modified but not necessary

// Exceptionally, JUCE will be replaced by a tiny lib...
//#include "../JuceLibraryCode/JuceHeader.h"
#include "RtMidi.h"

using namespace rack;

struct OnOffWidget : ModuleWidget
{
	RtMidiIn* rmi; // To be moved on the Module ?
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

	RtMidiIn *midiIn = 0;
	float LEDs[3] = {};
	void step();

	unsigned int dumpMidi(const std::string& nameLike);
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
inline unsigned sizeOfArray(const T(&)[size]) { return size; }

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
