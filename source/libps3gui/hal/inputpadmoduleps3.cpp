

#include "libps3gui/hal/inputpadmoduleps3.h"

#ifdef PSL1GHT
#include <io/pad.h>

void InputPadModulePS3::init( EPadNumber eNbPads, EPadCapability ePadCap)
{
	int iPadNumber = eNbPads + 1;

	if( eNbPads == ePadAll)
		iPadNumber = 7;

	if( ioPadInit(iPadNumber) == 0)
	{
		
	}
}
#endif

