
#ifndef MIDI_MAPPER_WIDGET
#define MIDI_MAPPER_WIDGET

// Care : modified but finally not necessary
//#include "engine.hpp"

#include "rack.hpp"

// The Widget should be în a separated file... And then one module per file as well
struct MidiMapperWidget : rack::ModuleWidget
{
	MidiMapperWidget();
	~MidiMapperWidget();
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

#endif // MIDI_MAPPER_WIDGET
