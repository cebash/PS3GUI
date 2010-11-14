
#include "videomodule.h"
#include "videomodulewii.h"
#include "videomoduleps3.h"


VideoModule * VideoModule::videoInstance = NULL;

VideoModule * VideoModule::getVideoModule( void)
{
	if( videoInstance == NULL)
	{
#ifdef DEVKITPPC
		videoInstance =  new VideoWii();
#elif defined PSL1GHT
		videoInstance =  new VideoModulePS3();
#else
		videoInstance = NULL;
#endif
	}
	return videoInstance;
}


void VideoModule::init(void)
{
#ifdef PSL1GHT
	initializeGPU();
#endif

	setupScreenMode();

	initializeDoubleBuffer();

#ifdef DEVKITPPC
	initializeGPU();
#endif

}

