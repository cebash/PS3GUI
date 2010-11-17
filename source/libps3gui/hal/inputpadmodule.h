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

class Pad
{
};

class InputPadModule
{
protected:
	static InputPadModule * inputPadInstance;

	InputPadModule() {}

	std::vector<Pad *> _pads;	
public:
	~InputPadModule() {}

	virtual void init( EPadNumber eNbPads, EPadCapability ePadCap = ePadCapNone) = 0;

	Pad * getPad( size_t szPadNumber);
	size_t getPadNumber () {return _pads.size();}

	static InputPadModule * getInputPadModule( void);
};

