
#include "libps3gui/hal/inputpadmodule.h"
#include "libps3gui/hal/inputpadmodulewii.h"
#include "libps3gui/hal/inputpadmoduleps3.h"

InputPadModule * InputPadModule::inputPadInstance = 0;

void InputPadModule::clearPads(void)
{
	for(size_t i = 0; i < _pads.size(); ++i)
		delete _pads[i];

	_pads.clear();
}

Pad * InputPadModule::getPad( size_t szPadNumber)
{
	return ( szPadNumber <_pads.size() ? _pads[szPadNumber] : 0);
}

InputPadModule * InputPadModule::getInputPadModule( void)
{
	if( inputPadInstance == 0)
	{
#ifdef DEVKITPPC
		inputPadInstance = (InputPadModule*) new InputPadModuleWii();
#else
		inputPadInstance = new InputPadModulePS3();
#endif
	}
	return inputPadInstance;
}


