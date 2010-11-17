#pragma once

#include "libps3gui/hal/inputpadmodule.h"


class InputPadModulePS3 : public InputPadModule
{
protected:

public:
	InputPadModulePS3() {}
	~InputPadModulePS3() {}

	virtual void init( EPadNumber eNbPads, EPadCapability ePadCap = ePadCapNone);

};

