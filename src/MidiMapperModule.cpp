
#include "MidiMapperModule.hpp"
#include "ConfigurationFiles.hpp" // Little helper .ini file reader
//#include "MidiMapperPlugin.hpp" // Care circular ref !
#include "TinyLogger.hpp"
// Rack
#include "util.hpp"
// System
#include <ctime>
#include <sstream>
#include <string>
#include <map>
#include <limits>

// Exceptionally, JUCE will be replaced here by a tiny lib...
//#include "../JuceLibraryCode/JuceHeader.h"
#include "RtMidi.h"

using namespace rack;

extern Plugin *thisPlugin;

MidiMapperModule *thisModule = 0;
ParamWidget *thisMapSwitch = 0;
ParamWidget *thisLearnSwitch = 0;
RtMidiIn *rtmidiIn = 0;

//ConfigurationFile cf("/home/sam/Rack/plugins/MidiMapper/MidiMapper.ini");
ConfigurationFile *cf = 0;

// The key here have the same format as in the configuration file : module_name.parameter_index
std::map<std::string, QuantityWidget*> parameters;

// Can't understand why there is no shorter way to perform this conversion...
std::string intToStr(int value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string hexToStr(int value)
{
	std::stringstream ss;
	ss << std::hex << value;
	return ss.str();
}

void MidiMapperModule::dumpParam(QuantityWidget* w)
{
	LOGF("value = %f; minValue = %f; maxValue = %f; defaultValue = %f; text = %s", w->value, w->minValue, w->maxValue, w->defaultValue, w->getText().c_str());
}

void MidiMapperModule::dumpParameters()
{
	for (auto const &kvp : parameters)
	{
		LOGF("Found a match for key = %s", kvp.first.c_str());
	}
}

void MidiMapperModule::dumpRack()
{
	if (gRackWidget == NULL)
	{
		logf("Argh leaving early !-)");
		return;
	}

  parameters.clear();
  std::string key;

	for (Widget *w : gRackWidget->moduleContainer->children)
	{
		logf("Found ModuleWidget !");
		ModuleWidget *moduleWidget = dynamic_cast<ModuleWidget*>(w);

		logf("Found model(aka module) :");
		logf(moduleWidget->model->slug.c_str());

		// Care struct Param just holds a float.µ
		// Check struct ParamWidget which inherit from QuantityWidget then call getText
		// And maybe add some missing getters !
		for (size_t i = 0; i < moduleWidget->params.size(); i++)
		{
			LOGF("Found param[%d]", i);
			dumpParam(moduleWidget->params[i]);
			// This compile but Parameter does not have name :( And the getText method return the value
			//logf(moduleWidget->params[i]->label.c_str());
			// Sample call :
			// addParam(createParam<NKK>(Vec(14, 129), module, p0wr::MAP_PARAM, 0.0, 1.0, 0.0));

			key = moduleWidget->model->slug + "." + intToStr(i);
			if (cf->valueExists("Mapping", key))
			{
				parameters[key] = moduleWidget->params[i]; // Store a "direct" pointer do the pot...
			}
		}
	}

	dumpParameters();
}

void MidiMapperModule::dumpConfig()
{
	logf("Entering MidiMapperModule::dumpConfig");
  for (std::string& section : cf->sections())
  {
		for (std::string& key : cf->keyNames(section))
		{
			LOGF("[%s]%s=%s", section.c_str(), key.c_str(), cf->keyValue(section, key).c_str());
		}
  }
  logf("Leaving MidiMapperModule::dumpConfig");
}

/*
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
*/

MidiMapperModule::MidiMapperModule() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) // Fix the bloody warning ! And strange bug in the Widget
{
	logf("Entering MidiMapperModule ctor");

	// After this, the tiny logger will be able to apply filtering
	cf = new ConfigurationFile(assetPlugin(thisPlugin, "MidiMapper.ini"));

	dumpConfig();
	dumpRack();
	initSniff(false);

	logf("Leaving MidiMapperModule ctor");
}

MidiMapperModule::~MidiMapperModule()
{
	logf("Entering MidiMapperModule dtor");
	cf->save(assetPlugin(thisPlugin, "session.ini"));
	delete cf;
	cf = 0;
	logf("Leaving MidiMapperModule dtor");
}

void midiInCallback(double deltatime, std::vector<unsigned char> *message, void *dummy)
{
	unsigned int nBytes = message->size();
	for (unsigned int i=0; i<nBytes; i++)
	{
		LOGF("Midi input byte[%u] = %u", i, (unsigned int)message->at(i));
	}

	LOGF("nBytes = %u", nBytes);
	if ((nBytes == 3) and (((char) message->at(0)) == '\xB0'))
	{
		std::string key = hexToStr((int) message->at(1));
		std::string value = cf->keyValue("Mapping", key);
		std::string sf = cf->keyValue("SignFactor", key);
		LOGF("key = %s; value = %s; sf = %s", key.c_str(), value.c_str(), sf.c_str());

		if (thisModule->m_mapMode && value != "")
		{
			LOGF("key = %s found ! value = %s", key.c_str(), value.c_str());

			int midiValue = (int) (message->at(2));
			if (sf == "1")
			{
				midiValue -= 64;
			}

			// ((127 - 0) or (64 to -63)) * midiValue = (max - min) / outValue ==> outValue = (midiVale * (max - min)) / (127 or 127 :)
			float outValue = (midiValue * (parameters[value]->maxValue - parameters[value]->minValue)) / 127;
			LOGF("midiValue = %d; parameters[value]->maxValue = %f; parameters[value]->minValue = %f; outValue = %f", midiValue, parameters[value]->maxValue, parameters[value]->minValue, outValue);

			/*
			if (sf != "")
			{
				LOGF("outValue = %f", outValue);
				outValue *= std::stof(sf);
			}
			*/

			LOGF("outValue = %f", outValue);
			parameters[value]->setValue(outValue); //TODO: Verify that the control still exist !
		} // else : key not mapped ==> Nothing to do !

		if (thisModule->m_learnMode && thisModule->paramWidgetFound)
		{
			//TODO: Associate !
		}

		// We discard midi input if we are not in m_mapMode nor m_learnMode !
	}
}

// Return the index of the midi device or maxint. Care rtmidiIn must be created before !
unsigned int MidiMapperModule::dumpMidiDevices(const std::string& nameLike)
{
	unsigned int i, result;
	unsigned int portCount = rtmidiIn->getPortCount();
	std::string portName;

	LOGF("MidiMapperModule::dumpMidiDevices => Entering %u", portCount);
	result = std::numeric_limits<unsigned int>::max();
	for (i=0; i<portCount; i++)
	{
		portName = rtmidiIn->getPortName(i);
		LOGF("Midi input %u = %s", i, portName.c_str());

		if (portName.find(nameLike) != std::string::npos)
		{
			logf("Midi device found");
			result = i;
		}
	}

	LOGF("MidiMapperModule::dumpMidiDevices => Leaving result = %u", result);
	return result;
}

void MidiMapperModule::startListen(unsigned int index)
{
	if (index == std::numeric_limits<unsigned int>::max()) return;

	logf("MidiMapperModule::startListen => Entering");
	rtmidiIn->openPort(index);
	rtmidiIn->setCallback(&midiInCallback);
	rtmidiIn->ignoreTypes(false, false, false);
	logf("MidiMapperModule::startListen => Leaving");
}

void MidiMapperModule::stopListen()
{
	logf("MidiMapperModule::stopListen => Entering");
	rtmidiIn->closePort();
	delete rtmidiIn;
	rtmidiIn = 0;
	logf("MidiMapperModule::stopListen => Leaving");
}

// Care : we are here at a bad place for introspection... Called tons of time !
void MidiMapperModule::step()
{
	switch ((int)roundf(params[MAP_PARAM].value))
	{
		case 0: // Map off
		{
			if (m_mapMode)
			{
				m_mapMode = false;
				thisLearnSwitch->visible = true;

				lights[M_RED_LIGHT].value = 0;
				lights[M_YEL_LIGHT].value = 0;
				lights[M_BLU_LIGHT].value = 0;

				stopListen();
			}
			break;
		}

		case 1: // Map On
		{
			if (!m_mapMode)
			{
				m_mapMode = true;
			  thisLearnSwitch->visible = false;

				lights[M_RED_LIGHT].value = 1;
				lights[M_YEL_LIGHT].value = 1;
				lights[M_BLU_LIGHT].value = 1;

				rtmidiIn = new RtMidiIn(); // Care dumpMidi needs it !
				startListen(dumpMidiDevices(cf->keyValue("MidiInDevice", "NameLike")));
			}
			break;
		}
	}

	switch ((int)roundf(params[LEARN_PARAM].value))
	{
		case 0: // Learn off
		{
			if (m_learnMode)
			{
				m_learnMode = false;
				thisMapSwitch->visible = true;

				lights[L_RED_LIGHT].value = 0;
				lights[L_YEL_LIGHT].value = 0;
				lights[L_BLU_LIGHT].value = 0;

				stopListen();
			}
			break;
		}

		case 1: // Learn on
		{
			if (!m_learnMode)
			{
				m_learnMode = true;
				thisMapSwitch->visible = false;

				lights[L_RED_LIGHT].value = 1;
				lights[L_YEL_LIGHT].value = 1;
				lights[L_BLU_LIGHT].value = 1;

				rtmidiIn = new RtMidiIn(); // Care dumpMidiDevices needs it !
				startListen(dumpMidiDevices(cf->keyValue("MidiInDevice", "NameLike")));

				initSniff(false);
				startSniff();
			}

			break;
		}
	}
}

// VCO sniffer part :)
void MidiMapperModule::initSniff(bool learning)
{
	timeoutSeconds = 0;
	holdPositionSeconds = 0;
	stepCounter = 0;
	animateCounter = 0;
	stepCounter = 0;
	isLearning = learning;
	hasFound = false;
	paramWidgetFound = nullptr;
	mouseAreaPrevious.pos.x = 0.0;
	mouseAreaPrevious.pos.y = 0.0;
	mouseAreaPrevious.size.y = 0.0;
	mouseAreaPrevious.size.y = 0.0;
}

void MidiMapperModule::startSniff()
{
	if (isLearning)
	{
			if (++stepCounter >= engineGetSampleRate()) // we do something only once per second...
			{
					stepCounter=0;
					if (++timeoutSeconds >= 10) // 10 seconds is a timeout on our sniff... sniff sniff
					{
							initSniff(false);
							logf("Not found :(");
					}
					else
					{
							if (mouseAreaPrevious.contains(gRackWidget->lastMousePos))
							{
									if (++holdPositionSeconds >= 3) // Time to find !
									{
											int moduleContainerIdx=0;
											for (Widget *w : gRackWidget->moduleContainer->children)
											{
													ModuleWidget *moduleWidget = dynamic_cast<ModuleWidget*>(w);
													for (size_t i = 0; (isLearning) && (i < moduleWidget->params.size()); i++)
													{
															if (moduleWidget->params[i]->visible)
															{
																	Rect paramWidgetAbsoluteBox = moduleWidget->params[i]->box;
																	paramWidgetAbsoluteBox.pos.x += moduleWidget->box.pos.x;
																	paramWidgetAbsoluteBox.pos.y += moduleWidget->box.pos.y;
																	if (paramWidgetAbsoluteBox.contains(gRackWidget->lastMousePos))
																	{
																			debug("[nethys] found parameter ! area = [%f, %f, %f, %f]", paramWidgetAbsoluteBox.pos.x,
																																																	paramWidgetAbsoluteBox.pos.y,
																																																	paramWidgetAbsoluteBox.pos.x + paramWidgetAbsoluteBox.size.x,
																																																	paramWidgetAbsoluteBox.pos.y + paramWidgetAbsoluteBox.size.y);
																			debug("[nethys] paramId = %d of module #%d in rack , instance of %s/%s", moduleWidget->params[i]->paramId,
																																																							moduleContainerIdx,
																																																							moduleWidget->model->slug.c_str(),
																																																							moduleWidget->model->name.c_str());
																			isLearning = false;
																			hasFound = true;
																			paramWidgetFound = moduleWidget->params[i];
																			timeoutSeconds = 0;
																			holdPositionSeconds = 0;
																			animateCounter = 4; // 4 x 1/2 seconds to animate parametre widget
																			logf("Pot Found !");
																			break;
																	}
															}
													}
													moduleContainerIdx++;
											}
									} // holdPositionSeconds
							}
							else
							{
									/*
									debug("[nethys] last mouse pos = [%f, %f]", gRackWidget->lastMousePos.x, gRackWidget->lastMousePos.y);
									debug("[nethys] last area = [%f, %f, %f, %f]", mouseAreaPrevious.pos.x,
																																	mouseAreaPrevious.pos.y,
																																	mouseAreaPrevious.pos.x + mouseAreaPrevious.size.x,
																																	mouseAreaPrevious.pos.y + mouseAreaPrevious.size.y);
									*/
									// we'll check that our mouse pointer remains in a area of +-5 points
									mouseAreaPrevious.pos.x = gRackWidget->lastMousePos.x-5.0;
									mouseAreaPrevious.pos.y = gRackWidget->lastMousePos.y-5.0;
									mouseAreaPrevious.size.x = 10.0;
									mouseAreaPrevious.size.y = 10.0;
									holdPositionSeconds = 0;
							}
					}

			} // stepcounter
	} // isLearning
	else
	{
			if (hasFound)
			{
					if (paramWidgetFound)
					{
							// "animate" the widget of the parameter just found (hide/show/hide...)
							if (++stepCounter >= engineGetSampleRate() / 2) // we do something only once per 1/2 second...
							{
									stepCounter=0;
									if (--animateCounter > 0)
									{
											if (paramWidgetFound->visible)
													paramWidgetFound->visible = false;
											else
													paramWidgetFound->visible = true;
									}
									else
									{
											paramWidgetFound->visible = true;
											hasFound = false;
											paramWidgetFound = NULL;
											animateCounter = 0;
									}
							}
					}
					else
					{
							hasFound = false;
							animateCounter = 0;
					}
			}
			else
			{
				initSniff(true);
				logf("Sniffing");
			}
	}
}
