#pragma once

#include <vector>

typedef enum 
{
	ePadAll = -1,
	ePad1,
	ePad2,
	ePad3,
	ePad4
} EPadNumber;


typedef enum 
{
	ePadCapNone 		= 0,
	ePadCapAccelerator 	= 1,
	ePadCapIR			= 2,
	ePadCapRumble		= 4
} EPadCapability;

typedef enum
{
	eAbsButNone,
	eAbsButExecute,
	eAbsButBack,
	eAbsButOption,
	eAbsButHome,
	eAbsButRight,
	eAbsButUp,
	eAbsButLeft,
	eAbsButDown,
} EAbstractButton; 

class Pad
{
protected:
	EPadCapability ePadCap;
public:
	virtual EPadCapability getCapabilty() {return ePadCap;}

	virtual bool isPressed( EAbstractButton eAbstractButton) = 0;
};

class InputPadModule
{
protected:
	static InputPadModule * inputPadInstance;

	InputPadModule() {}

	std::vector<Pad *> _pads;	
	void clearPads(void);

public:
	~InputPadModule() {}

	virtual void init( EPadNumber eNbPads, EPadCapability ePadCap = ePadCapNone) = 0;
	virtual void refreshPadConnections() = 0;

	Pad * getPad( size_t szPadNumber);
	size_t getPadNumber () {return _pads.size();}

	static InputPadModule * getInputPadModule( void);
};

