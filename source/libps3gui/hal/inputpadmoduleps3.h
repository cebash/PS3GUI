#pragma once

#include "libps3gui/hal/inputpadmodule.h"

#include <io/pad.h>


class PS3Pad : public Pad
{
protected:
	PadData _clPadData;
	int		_iPadNumber;
public:
	PS3Pad( int iPadNumber) { _iPadNumber = iPadNumber;}
	virtual ~PS3Pad() {}

	virtual bool isPressed( EAbstractButton eAbstractButton);
	
};

class InputPadModulePS3 : public InputPadModule
{
protected:

public:
	InputPadModulePS3(){}
	~InputPadModulePS3() {}

	virtual void init( EPadNumber eNbPads, EPadCapability ePadCap = ePadCapNone);

	virtual void refreshPadConnections();
};

