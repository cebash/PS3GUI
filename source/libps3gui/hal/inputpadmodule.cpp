
#include "inputpadmodule.h"
#include "inputpadmodulewii.h"

#if defined PSL1GHT
#include <io/pad.h>
#endif


InputPadModule * InputPadModule::inputPadInstance = 0;

InputPadModule * InputPadModule::getInputPadModule( void)
{
	if( inputPadInstance == 0)
	{
#ifdef DEVKITPPC
		inputPadInstance =  new InputPadModuleWii();
#elif defined PSL1GHT
		inputPadInstance =  new InputPadModulePS3();
#else
		inputPadInstance = 0;
#endif
	}
	return inputPadInstance;
}


void InputPadModule::init( EPadNumber eNumberPads)
{
#ifdef DEVKITPPC
	InputPadModuleWii ipmw = (InputPadModuleWii) inputpadmodule;

	ipmw->setup( iNumberPads, true, true);
#elif defined PSL1GHT
#endif

}

