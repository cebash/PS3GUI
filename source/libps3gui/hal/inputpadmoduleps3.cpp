

#include "libps3gui/hal/inputpadmoduleps3.h"

#ifdef PSL1GHT
#include <io/pad.h>


bool PS3Pad::isPressed( EAbstractButton eAbstractButton)
{
	bool bReturn = false;
	ioPadGetData(_iPadNumber, &_clPadData);
	switch( eAbstractButton)
	{
	default:
	case eAbsButExecute:
		bReturn = _clPadData.BTN_CROSS;
		break;
	}
	return bReturn;
}

void InputPadModulePS3::init( EPadNumber eNbPads, EPadCapability ePadCap)
{
	int iPadNumber = eNbPads + 1;

	if( eNbPads == ePadAll)
		iPadNumber = 7;

	ioPadInit(iPadNumber);
}

void InputPadModulePS3::refreshPadConnections()
{
	
	PadInfo padinfo;
	ioPadGetInfo(&padinfo);

	_pads.clear();

	int i;
	for(i=0; i<MAX_PADS; i++){
		if(padinfo.status[i]){

			PS3Pad * pclPad = new PS3Pad(i);
			_pads.push_back( pclPad);
		}

	}
}
#endif

