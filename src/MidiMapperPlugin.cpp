
#include "MidiMapperPlugin.hpp"
#include "MidiMapperWidget.hpp"
#include "TinyLogger.hpp"

#include "rack.hpp"
using namespace rack;

Plugin *thisPlugin;

void init(Plugin *p)
{
	thisPlugin = p; // Care : we need the plugin instance before logging :)
	logf("Entering init");

	p->slug = "(C)oded by Sam TFL/TDV";
	//plugin->name = "MidiMapper";

	#ifdef VERSION
	p->version = TOSTRING(VERSION);
	#endif
	p->website = "https://github.com/mderie/MidiMapper";
  //widget = createModel<MidiMapperWidget>("MidiMapper", "MidiMapper OnOff", "MidiMapper OnOff", UTILITY_TAG);
	//p->addModel(widget);
	logf("Before addModel");
	p->addModel(createModel<MidiMapperWidget>("MidiMapperModule", "MidiMapperModule", "MidiMapperModule", UTILITY_TAG));
	logf("After addModel");

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
        logf("Leaving init");
}
