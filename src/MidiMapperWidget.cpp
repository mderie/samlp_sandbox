
#include "MidiMapperWidget.hpp"
#include "MidiMapperModule.hpp"
//#include "MidiMapperPlugin.hpp" // Care circular ref !
#include "TinyLogger.hpp"

using namespace rack;

// Some globals... To rename !!!
extern Plugin *thisPlugin;
extern Module *thisModule;
extern ParamWidget *thisMapSwitch;
extern ParamWidget *thisLearnSwitch;

MidiMapperWidget::MidiMapperWidget()
{
	logf("MidiMapperWidget ctor ==> Entering");
  /*
	for (size_t i = 0; i < sizeOfArray(rotaryButtons); i++)
	{
		logf(rotaryButtons[i].c_str());
	}

	juce::StringArray midiInputDevices = juce::MidiInput::getDevices();
	for (int i = 0; i < midiInputDevices.size(); i++)
	{
		logf(midiInputDevices[i].toStdString().c_str());
	}
  */

  thisModule = new MidiMapperModule(); // Generate a strange warning... About deprecated :(
	//this->module = nullptr; // Fix ? for the random bug below... Or not :(
	setModule(thisModule);
	box.size = Vec(4 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT); // (4 * 15, 380)
	// AMHA : strange construction...
	{
		Panel *panel = new LightPanel();
		panel->backgroundImage = Image::load(assetPlugin(thisPlugin, "res/switch.png"));
		panel->box.size = box.size;
		addChild(panel);
	}
	/*
	{
		SVGPanel *panel = new SVGPanel();
		logf("step 1.6");
		panel->box.size = box.size;
		logf("step 1.7");
		panel->setBackground(SVG::load(assetPlugin(thisPlugin, "res/switch.png")));
		logf("step 1.8");
		addChild(panel);
	}
	*/

	thisMapSwitch = createParam<NKK>(Vec(14, 129), thisModule, MidiMapperModule::MAP_PARAM, 0.0, 1.0, 0.0); // NKK ?
	addParam(thisMapSwitch);
	addChild(createLight<LargeLight<RedLight>>(Vec(23,  54), thisModule, MidiMapperModule::M_RED_LIGHT));
	addChild(createLight<LargeLight<YellowLight>>(Vec(23,  78), thisModule, MidiMapperModule::M_YEL_LIGHT));
	addChild(createLight<LargeLight<BlueLight>>(Vec(23, 102), thisModule, MidiMapperModule::M_BLU_LIGHT));
	thisLearnSwitch = createParam<NKK>(Vec(14, 209), thisModule, MidiMapperModule::LEARN_PARAM, 0.0, 1.0, 0.0);
	addParam(thisLearnSwitch);
	addChild(createLight<LargeLight<RedLight>>(Vec(23,  134), thisModule, MidiMapperModule::L_RED_LIGHT));
	addChild(createLight<LargeLight<YellowLight>>(Vec(23,  158), thisModule, MidiMapperModule::L_YEL_LIGHT));
	addChild(createLight<LargeLight<BlueLight>>(Vec(23, 182), thisModule, MidiMapperModule::L_BLU_LIGHT));

	logf("MidiMapperWidget ctor ==> Leaving");
}

MidiMapperWidget::~MidiMapperWidget()
{
	logf("MidiMapperWidget dtor ==> Entering / Leaving");
}
