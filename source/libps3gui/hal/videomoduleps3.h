#pragma once

#include "videomodule.h"

#ifdef PSL1GHT

class VideoModulePS3 : public VideoModule
{
protected:
	gcmContextData *_CommandBuffer; // Context to keep track of the RSX buffer.	
	VideoResolution _resolution; // Screen Resolution

	int _currentBuffer;
	s32 *buffer[2]; // The buffer we will be drawing into.

	virtual void setupScreenMode( void);
	virtual void initializeGPU( void);
	virtual void initializeDoubleBuffer( void);
public:
	VideoModulePS3();
	virtual void stop( void);
	virtual void initConsole(void);

	virtual u32 getHeight(void) {return _resolution.height;} ;
	virtual u32 getWidth(void) {return _resolution.width;} ;

	virtual u32* getCurrentBuffer(void) {return (u32*)buffer[_currentBuffer];} ;

	virtual void flip( void);
	virtual void waitFlip();


	virtual void clearBackground( void);
	virtual void setupVertexDesc( void);

	virtual void displayBitmap( u32* bitmap, int height, int width, int wpitch);
	virtual void printf( u32 x, u32 y,  const char *);
};

#endif

