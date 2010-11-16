#pragma once

#include "inputpadmodule.h"

#ifdef DEVKITPPC

#include <wiiuse/wpad.h>
class InputPadModuleWii : public InputPadModule
{
protected:
	InputPadModuleWii() {}

public:
	void setup( int iNumberPads, bool enableAccel, bool enableIR);

};


#endif
