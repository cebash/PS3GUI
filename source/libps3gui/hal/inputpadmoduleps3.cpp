

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
	case eAbsButBack: 
		bReturn = _clPadData.BTN_CIRCLE;
		break;
	case eAbsButOption:
		bReturn = _clPadData.BTN_TRIANGLE;
		break;
	case eAbsButRight:
		bReturn = _clPadData.BTN_RIGHT;
		break;
	case eAbsButUp:
		bReturn = _clPadData.BTN_UP;
		break;
	case eAbsButLeft:
		bReturn = _clPadData.BTN_LEFT;
		break;
	case eAbsButDown:
		bReturn = _clPadData.BTN_DOWN;
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

	clearPads();

	for( int i = 0; i < MAX_PADS; i++){
		if(padinfo.status[i]){

			PS3Pad * pclPad = new PS3Pad(i);
			_pads.push_back( pclPad);
		}

	}
}
#endif

