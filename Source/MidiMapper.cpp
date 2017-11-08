
#include "MidiMapper.hpp"
#include <ctime>
#include <sstream>
#include <string>

// C like simple logger
void log(const char *s)
{
  time_t rawtime;
  struct tm* timeinfo;
  char timestamp[32]; // Sure there is more C++ way to do this...

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(timestamp, sizeof(timestamp), "%d/%m/%Y %H:%M:%S", timeinfo);

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

//TODO:
// Replace JUCE library by an alternate tiny midi one !

Plugin *plugin;

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

        // Go directly from the rack ! Maybe too early... gRackWidget is NULL now :(
        if (gRackWidget == NULL)
        {
          log("Argh leaving early !-)");
          return;
        }

        for (Widget *w : gRackWidget->moduleContainer->children)
        {
          log("Found ModuleWidget :");
          ModuleWidget *moduleWidget = dynamic_cast<ModuleWidget*>(w);

          log("Found model(aka module) :");
          log(moduleWidget->model->slug.c_str());
        }

        log("Leaving init");
}

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
	log("Leaving OnOff ctor");
}

OnOff::~OnOff()
{
	log("Entering / Leaving OnOffWidget dtor");
}

void OnOff::step()
{
	switch ((int)roundf(params[SWITCH_PARAM].value))
	{
		case 0:
		{
			//TODO
			break;
		}

		case 1:
		{
			//TODO
			break;
		}
	}
}

/*
void mycallback(double deltatime, std::vector<unsigned char> *message, void *dummy)
{
  unsigned int nBytes = message->size();
  for ( unsigned int i=0; i<nBytes; i++ )
    std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
  if ( nBytes > 0 )
    std::cout << "stamp = " << deltatime << std::endl;
}
*/

/*
void MIC::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
	log("Incomming message = ...");
	const juce::uint8* p = message.getRawData();
	std::string s;
	for (int i = 0; i < message.getRawDataSize(); i++)
	{
		s = intToStr(static_cast<int>(*(p + i)));
		log(s.c_str());
	}
}
*/
