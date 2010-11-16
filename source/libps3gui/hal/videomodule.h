#pragma once

#include "..//gui_types.h"

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

	virtual void flip(void) = 0;
	virtual void waitFlip(void) = 0;

	virtual void clearBackground( void) = 0;
	virtual void setupVertexDesc( void) = 0;


	static VideoModule * getVideoModule( void);
};

