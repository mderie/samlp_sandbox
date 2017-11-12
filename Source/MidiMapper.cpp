
// Mine
#include "MidiMapper.hpp"
#include "ConfigurationFiles.hpp"
// Rack
#include "util.hpp"
// System
#include <ctime>
#include <sstream>
#include <string>
#include <map>
#include <limits>

ConfigurationFile cf("/home/sam/Rack/plugins/MidiMapper/MidiMapper.ini");

// The key here have the same format as in the configuration file : module_name.parameter_index
std::map<std::string, QuantityWidget*> parameters;

#define LOGF(s, ...) log(stringf(s, __VA_ARGS__).c_str())

// C like simple logger
void log(const char *s)
{
	if (cf.keyValue("Misc", "Debug") != "1") return;

  time_t rawtime;
  struct tm* timeinfo;
  char timestamp[32]; // Sure there is more C++ way to do this...

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(timestamp, sizeof(timestamp), "%d/%m/%Y %H:%M:%S", timeinfo);

  //TODO: Get the current folder ?
  std::string fullFilename = "/home/sam/Rack/plugins/MidiMapper/MidiMapper.log";
  FILE *fLog = fopen(fullFilename.c_str(), "a");
  fprintf(fLog, "%s => %s\n", timestamp, s);
  fclose(fLog);
}

// Can't understand why there is no shorter way to perform this conversion...
std::string intToStr(int value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

Plugin *plugin;

void dumpParam(QuantityWidget* w)
{
	std::string s = stringf("value = %f; minValue = %f; maxValue = %f; defaultValue = %f; text = %s", w->value, w->minValue, w->maxValue, w->defaultValue, w->getText().c_str());
	log(s.c_str());
}

void dumpParameters()
{
	for (auto const &kvp : parameters)
	{
		log(stringf("Found a match for key = %s", kvp.first.c_str()).c_str());
	}
}

void dumpRack()
{
	if (gRackWidget == NULL)
	{
		log("Argh leaving early !-)");
		return;
	}

  parameters.clear();
  std::string key;

	for (Widget *w : gRackWidget->moduleContainer->children)
	{
		log("Found ModuleWidget !");
		ModuleWidget *moduleWidget = dynamic_cast<ModuleWidget*>(w);

		log("Found model(aka module) :");
		log(moduleWidget->model->slug.c_str());

		// Care struct Param just holds a float.µ
		// Check struct ParamWidget which inherit from QuantityWidget then call getText
		// And maybe add some missing getters !
		for (size_t i = 0; i < moduleWidget->params.size(); i++)
		{
			log(stringf("Found param[%d]", i).c_str());
			dumpParam(moduleWidget->params[i]);
			// This compile but Parameter does not have name :( And the getText method return the value
			//log(moduleWidget->params[i]->label.c_str());
			// Sample call :
			// addParam(createParam<NKK>(Vec(14, 129), module, p0wr::SWITCH_PARAM, 0.0, 1.0, 0.0));

			key = moduleWidget->model->slug + "." + intToStr(i);
			if (cf.valueExists("Mapping", key))
			{
				parameters[key] = moduleWidget->params[i];
			}
		}
	}

	dumpParameters();
}

void dumpConfig()
{
	log("Entering dumpConfig");
	//log(cf.keyValue("Misc","debug").c_str());
  for (std::string& section : cf.sections())
  {
		for (std::string& key : cf.keyNames(section))
		{
			log(cf.keyValue(section, key).c_str());
		}
  }
  log("Leaving dumpConfig");
}

void init(rack::Plugin *p)
{
	log("Entering init");
	plugin = p;
	plugin->slug = "MidiMapper";
	plugin->name = "MidiMapper";
	createModel<OnOffWidget>(plugin, "MidiMapper OnOff", "MidiMapper OnOff");

        // Not necessary... Strange visual bug !
	//createModel<MidiMapper>(plugin, "MidiMapper", "MidiMapper");

        // OK
        //rack::gSampleRate = 0;

        // OK modified : rack::modules wass static !
        /*
        for (Module *module : rack::modules)
        {
		//module->step();
                //log(module.name);
                for (Param param : params)
                {

                }
	}
        */

        // OK but stuck at params...
        /*
        for (Plugin* plugin : rack::gPlugins)
        {
           log("Found plugin :");
           log(plugin->slug.c_str());
           for (Model* model : plugin->models)
           {
             log("Found model(aka module) :");
             log(model->slug.c_str());

             //TODO: Loop on params !
           }
        }
        */

        dumpConfig();
        // Go directly from the rack ! Maybe too early... gRackWidget is NULL now :(
        dumpRack();
        log("Leaving init");
}

//TODO: Replace Power by MidiMapper
struct p0wrModeLight : ModeValueLight
{
	p0wrModeLight()
	{
		addColor(COLOR_BLACK_TRANSPARENT);
		addColor(COLOR_RED);
		addColor(COLOR_YELLOW);
		addColor(COLOR_BLUE);
	}
};

OnOffWidget::OnOffWidget()
{
	log("Entering OnOffWidget ctor");
	rmi = NULL;

        /*
	for (size_t i = 0; i < sizeOfArray(rotaryButtons); i++)
	{
		log(rotaryButtons[i].c_str());
	}

	juce::StringArray midiInputDevices = juce::MidiInput::getDevices();
	for (int i = 0; i < midiInputDevices.size(); i++)
	{
		log(midiInputDevices[i].toStdString().c_str());
	}
  */

	OnOff *module = new OnOff();
	setModule(module);
	box.size = Vec(15 * 4, 380);

	{
		Panel *panel = new LightPanel();
		panel->backgroundImage = Image::load("plugins/MidiMapper/res/switch.png");
		panel->box.size = box.size;
		addChild(panel);
	}

	addParam(createParam<NKK>(Vec(14, 129), module, OnOff::SWITCH_PARAM, 0.0, 1.0, 0.0)); // NKK ?

	addChild(createValueLight<LargeLight<p0wrModeLight>>(Vec(20,  51), &module->LEDs[0]));
	addChild(createValueLight<LargeLight<p0wrModeLight>>(Vec(20,  75), &module->LEDs[1]));
	addChild(createValueLight<LargeLight<p0wrModeLight>>(Vec(20, 100), &module->LEDs[2]));

	log("Leaving OnOffWidget ctor");
}

OnOffWidget::~OnOffWidget()
{
	log("Entering / Leaving OnOffWidget dtor");
}

OnOff::OnOff()
{
	log("Entering OnOff ctor");
	params.resize(NUM_PARAMS);
	inputs.resize(NUM_INPUTS);
	outputs.resize(NUM_OUTPUTS);

	dumpRack();
	log("Leaving OnOff ctor");
}

OnOff::~OnOff()
{
	log("Entering / Leaving OnOffWidget dtor");
}

void midiInCallback(double deltatime, std::vector<unsigned char> *message, void *dummy)
{
  unsigned int nBytes = message->size();
  for (unsigned int i=0; i<nBytes; i++)
  {
    //std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
    log(stringf("Midi input byte[%d] = %d", i, (int)message->at(i)).c_str());
	}

  if ((nBytes = 3) and (((char) message->at(0)) == '\xCC'))
  {
		std::string key = std::string(1, (char) message->at(1));
		std::string value = cf.keyValue("Mapping", key);
    if (value != "")
    {
			log(stringf("key = %s found ! value = %s", key.c_str(), value.c_str()).c_str());

			int midiValue = (int) message->at(2); // (127-0)/midiValue = (max-min)/y ==> y = (midiVale*(max-min))/127
			parameters[value]->setValue((midiValue * (parameters[value]->maxValue - parameters[value]->minValue)) / 127);
    }
	}
}

unsigned int OnOff::dumpMidi(const std::string& nameLike)
{
	unsigned int i, result;
	unsigned int portCount = midiIn->getPortCount();
	std::string portName;

	LOGF("dumpMidi => Entering %u", portCount);
	result = std::numeric_limits<unsigned int>::max();
	for (i=0; i<portCount; i++)
	{
    portName = midiIn->getPortName(i);
    log(stringf("Midi input %u = %s", i, portName.c_str()).c_str());

    if (portName.find(nameLike) != std::string::npos)
    {
			log("Midi device found");
			result = i;
		}
  }

  log(stringf("dumpMidi => Leaving result = %u", result).c_str());
  return result;
}

void OnOff::startListen(unsigned int index)
{
	if (index == std::numeric_limits<unsigned int>::max()) return;

	log("startListen => Entering");
	midiIn->openPort(index);
	midiIn->setCallback(&midiInCallback);
	midiIn->ignoreTypes(false, false, false);
	log("startListen => Leaving");
}

void OnOff::stopListen()
{
	log("stopListen => Entering");
	midiIn->closePort();
	delete midiIn;
	midiIn = 0;
	log("stopListen => Leaving");
}

void OnOff::step()
{
	switch ((int)roundf(params[SWITCH_PARAM].value))
	{
		case 0:
		{
			//TODO: Off
			LEDs[0] = 0;
			LEDs[1] = 0;
			LEDs[2] = 0;

			if (midiIn)
			{
				stopListen();
			}
			break;
		}
		case 1:
		{
			//TODO: On but bad place for introspection... Called tons of time
			LEDs[0] = 1;
			LEDs[1] = 2;
			LEDs[2] = 3;

			if (!midiIn)
			{
				midiIn = new RtMidiIn();
				startListen(dumpMidi(cf.keyValue("MidiInDevice", "NameLike")));
			}
			break;
		}
	}
}
