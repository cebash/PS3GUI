
#include "inputpadmodulewii.h"
#ifdef DEVKITPPC
#include <gccore.h>
#include <ogcsys.h>
#include <wiiuse/wpad.h>
#include <io/pad.h>

void InputPadModule::init( EPadNumber eNbPads, EPadCapability ePadCap)
setup( size_t szNumberPads, bool enableAccel, bool enableIR)
{
	PAD_Init();
	WPAD_Init();

	// read wiimote accelerometer and IR data
	WPAD_SetDataFormat(WPAD_CHAN_ALL,WPAD_FMT_BTNS_ACC_IR);
	WPAD_SetVRes(WPAD_CHAN_ALL, screenwidth, screenheight);

	for(int i=0; i < 4; i++)
	{
		userInput[i].chan = i;
		userInput[i].wpad = WPAD_Data(i);
	}
}


#endif
