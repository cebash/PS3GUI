#pragma once

#include "libps3gui/gui_types.h"

class VideoModule {
protected:
	static VideoModule * videoInstance;

	virtual void setupScreenMode( void) = 0;
	virtual void initializeGPU( void) = 0;
	virtual void initializeDoubleBuffer( void) = 0;

public:
	virtual void init(void);
	virtual void stop() = 0;

	virtual void flip() = 0;
	virtual void waitFlip() = 0;

	virtual void clearBackground( void) = 0;
	virtual void setupVertexDesc( void) = 0;


	static VideoModule * getVideoModule( void);
};

#ifdef DEVKITPPC

#define DEFAULT_FIFO_SIZE 256 * 1024

class VideoWII : public VideoModule {
protected:
	unsigned char _CommandBuffer[DEFAULT_FIFO_SIZE] ATTRIBUTE_ALIGN (32);

	unsigned int *xfb[2] = { NULL, NULL }; // Double buffered
	int _currentBuffer = 0; // Switch

	GXRModeObj *_resolution; // Menu video mode
	Mtx GXmodelView2D;

	virtual void setupScreenMode( void);
	virtual void initializeGPU( void);
	virtual void initializeDoubleBuffer( void);

public:
	virtual void stop( void);

	virtual void flip( void);
	virtual void waitFlip();

	virtual void clearBackground( void);
	virtual void setupVertexDesc( void);
};

#elif defined PSL1GHT
class VideoPS3 : public VideoModule
{
protected:
	gcmContextData *_CommandBuffer; // Context to keep track of the RSX buffer.	
	VideoResolution _resolution; // Screen Resolution

	int _currentBuffer = 0;
	s32 *buffer[2]; // The buffer we will be drawing into.

	virtual void setupScreenMode( void);
	virtual void initializeGPU( void);
	virtual void initializeDoubleBuffer( void);

public:
	virtual void stop( void);

	virtual void flip( void);
	virtual void waitFlip();


	virtual void clearBackground( void);
	virtual void setupVertexDesc( void);
};

#endif

