#pragma once

typedef enum 
{
	ePadAll = -1,
	ePad1,
	ePad2,
	ePad3,
	ePad4
} EPadNumber;


class InputPadModule
{
protected:
	static InputPadModule * inputPadInstance;

	InputPadModule() {}
public:

	virtual void init( EPadNumber eNbPads);

	static InputPadModule * getInputPadModule( void);
};
