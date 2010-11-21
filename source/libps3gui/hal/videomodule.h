#pragma once

#include "libps3gui/gui_types.h"

#include <sysutil/video.h>
#include <rsx/gcm.h>
#include <rsx/reality.h>

class VideoModule {
protected:
	static VideoModule * videoInstance;

	virtual void setupScreenMode( void) = 0;
	virtual void initializeGPU( void) = 0;
	virtual void initializeDoubleBuffer( void) = 0;

	VideoModule() {}
public:


	virtual void init(void);
	virtual void stop(void) = 0;
	virtual void initConsole(void) = 0;

	virtual u32 getHeight(void) = 0;
	virtual u32 getWidth(void) = 0;

	virtual u32* getCurrentBuffer(void) = 0;

	virtual void flip(void) = 0;
	virtual void waitFlip(void) = 0;

	virtual void clearBackground( void) = 0;
	virtual void setupVertexDesc( void) = 0;

	virtual void displayBitmap( u32* bitmap, int height, int width, int wpitch) = 0;

	static VideoModule * getVideoModule( void);

	virtual void printf( u32 x, u32 y,  const char *) = 0;
};

