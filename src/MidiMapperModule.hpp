
#ifndef MIDI_MAPPER_MODULE
#define MIDI_MAPPER_MODULE

// Care : modified but finally not necessary
//#include "engine.hpp"

#include "rack.hpp"
//using namespace rack;

struct MidiMapperModule : rack::Module
{
	enum ParamIds
	{
		MAP_PARAM,
		LEARN_PARAM,
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
		M_RED_LIGHT,
		M_YEL_LIGHT,
		M_BLU_LIGHT,
		L_RED_LIGHT,
		L_YEL_LIGHT,
		L_BLU_LIGHT,
		NUM_LIGHTS
	};

	// Global OnOff switches
	bool m_mapMode = false;
	bool m_learnMode = false;

	//float LEDs[3] = {};

	// VCO sniffer part :)
  bool isLearning;
  bool hasFound;
  int stepCounter;
  int animateCounter;
  int timeoutSeconds;
  int holdPositionSeconds;
  rack::Rect mouseAreaPrevious;
  rack::ParamWidget *paramWidgetFound;

	MidiMapperModule();
	~MidiMapperModule();
	// Relace ctor & dtor...
	//void prelude();
	//void postlude();

	void step() override;

	unsigned int dumpMidiDevices(const std::string& nameLike);
	void dumpRack();
	void dumpConfig();
	void dumpParam(rack::QuantityWidget* w);
	void dumpParameters();

	void startListen(unsigned int index);
	void stopListen();

	void initSniff(bool learning);
	void startSniff();

	//bool mapMode { return m_mapMode; }
	//bool learnMode { return m_learnMode; }
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

#endif // MIDI_MAPPER_MODULE
