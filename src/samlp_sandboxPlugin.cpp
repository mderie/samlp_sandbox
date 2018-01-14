
#include "samlp_sandboxPlugin.hpp"
#include "MidiMapperWidget.hpp"
#include "ConsoleWidget.hpp"
#include "ConfigurationFiles.hpp"
#include "TinyLogger.hpp"

#include "rack.hpp"
using namespace rack;

// Some globals...
Plugin *thisPlugin = 0;
ConfigurationFile *cf = 0; // ConfigurationFile cf("/home/sam/Rack/plugins/MidiMapper/MidiMapper.ini");

void init(Plugin *p)
{
	thisPlugin = p; // Care : we need the plugin instance before logging :)

	// After this, the tiny logger will be able to apply filtering
	cf = new ConfigurationFile(assetPlugin(thisPlugin, "MidiMapper.ini"));
	logf("init ==> Entering");

	p->slug = "(C)oded by Sam TFL/TDV";
	//plugin->name = "MidiMapper";

	#ifdef VERSION
	p->version = TOSTRING(VERSION);
	#endif
	p->website = "https://github.com/mderie/MidiMapper";
  //widget = createModel<MidiMapperWidget>("MidiMapper", "MidiMapper OnOff", "MidiMapper OnOff", UTILITY_TAG);
	//p->addModel(widget);
	logf("init ==> Before addModel");
	p->addModel(createModel<MidiMapperWidget>("MidiMapperModule", "MidiMapperModule", "MidiMapperModule", UTILITY_TAG));
	logf("init ==> After addModel");

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
           }
        }
        */

        // Go directly from the rack ! Maybe too early... gRackWidget is NULL now :(
        //dumpRack();
	logf("init ==> Leaving");
}
