#pragma once

#include "videomodule.h"

#ifdef DEVKITPPC

#define DEFAULT_FIFO_SIZE 256 * 1024

class VideoModuleWII : public VideoModule {
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

#endif

