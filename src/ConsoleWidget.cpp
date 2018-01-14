
#include "ConsoleWidget.hpp"
#include "ConsoleModule.hpp"
#include "TinyLogger.hpp"

using namespace rack;

extern Plugin *thisPlugin;

ConsoleWidget::ConsoleWidget()
{
  logf("ConsoleWidget ctor ==> Entering");

  ConsoleModule *cm = new ConsoleModule();
  setModule(cm);
  box.size = Vec(16 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT); // (4 * 15, 380)
  {
    Panel *panel = new LightPanel();
    panel->backgroundImage = Image::load(assetPlugin(thisPlugin, "res/switch.png"));
    panel->box.size = box.size;
    addChild(panel);
  }

  /*
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
  */

  logf("ConsoleWidget ctor ==> Leaving");
}

ConsoleWidget::~ConsoleWidget()
{
	logf("ConsoleWidget dtor ==> Entering / Leaving");
}
