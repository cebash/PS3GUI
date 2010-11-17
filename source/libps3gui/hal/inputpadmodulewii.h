#pragma once

#include "inputpadmodule.h"

#ifdef DEVKITPPC

class InputPadModuleWii : public InputPadModule
{
protected:

public:

	virtual void init( EPadNumber eNbPads, EPadCapability ePadCap);

};

#endif

