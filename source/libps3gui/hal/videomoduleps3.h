#pragma once

#include "videomodule.h"

#ifdef PSL1GHT
#include <sysutil/video.h>
#include <rsx/gcm.h>
#include <rsx/reality.h>

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

	virtual void flip( void);
	virtual void waitFlip();


	virtual void clearBackground( void);
	virtual void setupVertexDesc( void);
};

#endif

