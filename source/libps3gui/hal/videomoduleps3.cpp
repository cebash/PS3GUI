
#include <psl1ght/lv2.h>

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include <sysutil/video.h>
#include <rsx/gcm.h>
#include <rsx/reality.h>

#include <psl1ght/lv2.h>

#include "videomoduleps3.h"


VideoModulePS3::VideoModulePS3()
{
	_currentBuffer = 0;
	buffer[0] = 0;
	buffer[1] = 0;
	_CommandBuffer = 0;
}

#ifdef PSL1GHT
void VideoModulePS3::initializeGPU(void)
{
   // Allocate a 1Mb buffer, alligned to a 1Mb boundary to be our shared IO memory with the RSX.
    void *host_addr = memalign(1024*1024, 1024*1024);
    assert(host_addr != NULL);

    // Initilise Reality, which sets up the command buffer and shared IO memory
    _CommandBuffer = realityInit(0x10000, 1024*1024, host_addr);
    assert(_CommandBuffer != NULL);
}

void VideoModulePS3::setupScreenMode(void)
{
    VideoState state;
    assert(videoGetState(0, 0, &state) == 0); // Get the state of the display
    assert(state.state == 0); // Make sure display is enabled

    // Get the current resolution
    assert(videoGetResolution(state.displayMode.resolution, &_resolution) == 0);

    // Configure the buffer format to xRGB
    VideoConfiguration vconfig;
    memset(&vconfig, 0, sizeof(VideoConfiguration));
    vconfig.resolution = state.displayMode.resolution;
    vconfig.format = VIDEO_BUFFER_FORMAT_XRGB;
    vconfig.pitch = _resolution.width * 4;

    assert(videoConfigure(0, &vconfig, NULL, 0) == 0);
    assert(videoGetState(0, 0, &state) == 0);

    gcmSetFlipMode(GCM_FLIP_VSYNC); // Wait for VSYNC to flip
}

void VideoModulePS3::initializeDoubleBuffer(void)
{
    s32 buffer_size = 4 * _resolution.width * _resolution.height; // each pixel is 4 bytes
    printf("buffers will be 0x%x bytes\n", buffer_size);

    // Allocate two buffers for the RSX to draw to the screen (double buffering)
    buffer[0] = (s32 *) rsxMemAlign(16, buffer_size);
    buffer[1] = (s32 *) rsxMemAlign(16, buffer_size);
    assert(buffer[0] != NULL && buffer[1] != NULL);

    u32 offset[2];
    assert(realityAddressToOffset(buffer[0], &offset[0]) == 0);
    assert(realityAddressToOffset(buffer[1], &offset[1]) == 0);
    // Setup the display buffers
    assert(gcmSetDisplayBuffer(0, offset[0], _resolution.width * 4, _resolution.width, _resolution.height) == 0);
    assert(gcmSetDisplayBuffer(1, offset[1], _resolution.width * 4, _resolution.width, _resolution.height) == 0);

    gcmResetFlipStatus();
}

void VideoModulePS3::flip( void)
{
    assert(gcmSetFlip(_CommandBuffer, _currentBuffer) == 0);
    realityFlushBuffer(_CommandBuffer);
    gcmSetWaitFlip(_CommandBuffer); // Prevent the RSX from continuing until the flip has finished.

	_currentBuffer ^= 1; // flip framebuffer
}

void VideoModulePS3::waitFlip( void)
{
	while(gcmGetFlipStatus() != 0)
        usleep(200);
    gcmResetFlipStatus();
}

void VideoModulePS3::clearBackground( void)
{
	s32 i, j;
    for(i = 0; i < _resolution.height; i++) {
        s32 color = 0;
        for(j = 0; j < _resolution.width; j++)
            (buffer[_currentBuffer])[i* _resolution.width + j] = color;
    }

}

void VideoModulePS3::setupVertexDesc( void)
{
}

void VideoModulePS3::stop( void)
{

}
#endif

