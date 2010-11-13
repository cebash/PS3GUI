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

#include "video_ps3.h"

VideoModule * VideoModule::videoInstance = NULL;

VideoModule * VideoModule::getVideoModule( void)
{
	if( videoInstance == NULL)
	{
#ifdef DEVKITPPC
		videoInstance =  new VideoWii();
#elif defined PSL1GHT
		videoInstance =  new VideoPS3();
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


#ifdef DEVKITPPC

void VideoWII::setupScreenMode( void)
{
	VIDEO_Init();
	_resolution = VIDEO_GetPreferredMode(NULL); // get default video mode

	// widescreen fix
	if(CONF_GetAspectRatio() == CONF_ASPECT_16_9)
		_resolution->viWidth = VI_MAX_WIDTH_PAL;

	VIDEO_Configure (_resolution);

}


void VideoWII::initializeDoubleBuffer( void)
{
	// Allocate the video buffers
	xfb[0] = (u32 *) MEM_K0_TO_K1 (SYS_AllocateFramebuffer (_resolution));
	xfb[1] = (u32 *) MEM_K0_TO_K1 (SYS_AllocateFramebuffer (_resolution));

	// A console is always useful while debugging
	console_init (xfb[0], 20, 64, _resolution->fbWidth, _resolution->xfbHeight, _resolution->fbWidth * 2);

	// Clear framebuffers etc.
	VIDEO_ClearFrameBuffer (_resolution, xfb[0], COLOR_BLACK);
	VIDEO_ClearFrameBuffer (_resolution, xfb[1], COLOR_BLACK);
	VIDEO_SetNextFramebuffer (xfb[0]);

	VIDEO_SetBlack (FALSE);
	VIDEO_Flush ();
	VIDEO_WaitVSync ();
	if (_resolution->viTVMode & VI_NON_INTERLACE)
		VIDEO_WaitVSync ();
}


void VideoWII::initializeGPU( void)
{
	// Initialize GX
	GXColor background = { 0, 0, 0, 0xff };
	memset (&_CommandBuffer, 0, DEFAULT_FIFO_SIZE);
	GX_Init (&_CommandBuffer, DEFAULT_FIFO_SIZE);
	GX_SetCopyClear (background, 0x00ffffff);
	GX_SetDispCopyGamma (GX_GM_1_0);
	GX_SetCullMode (GX_CULL_NONE);
}


void VideoWII::flip( void)
{
	_currentBuffer ^= 1; // flip framebuffer
	GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GX_SetColorUpdate(GX_TRUE);
	GX_CopyDisp(xfb[_currentBuffer],GX_TRUE);
	GX_DrawDone();
	VIDEO_SetNextFramebuffer(xfb[_currentBuffer]);
	VIDEO_Flush();
	VIDEO_WaitVSync();
}
void waitFlip()
{
	Mtx44 p;
	f32 yscale;
	u32 xfbHeight;

	VIDEO_Configure (_resolution);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if (_resolution->viTVMode & VI_NON_INTERLACE)
		VIDEO_WaitVSync();
	else
		while (VIDEO_GetNextField())
			VIDEO_WaitVSync();
}

void VideoWII::clearBackground( void)
{
	GXColor background = {0, 0, 0, 255};
	GX_SetCopyClear (background, GX_MAX_Z24);

	yscale = GX_GetYScaleFactor(_resolution->efbHeight,_resolution->xfbHeight);
	xfbHeight = GX_SetDispCopyYScale(yscale);
	GX_SetScissor(0,0,_resolution->fbWidth,_resolution->efbHeight);
	GX_SetDispCopySrc(0,0,_resolution->fbWidth,_resolution->efbHeight);
	GX_SetDispCopyDst(_resolution->fbWidth,xfbHeight);
	GX_SetCopyFilter(_resolution->aa,_resolution->sample_pattern,GX_TRUE,_resolution->vfilter);
	GX_SetFieldMode(_resolution->field_rendering,((_resolution->viHeight==2*_resolution->xfbHeight)?GX_ENABLE:GX_DISABLE));

	if (_resolution->aa)
		GX_SetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
	else
		GX_SetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);

}

void VideoWII::setupVertexDesc( void)
{
	// setup the vertex descriptor
	// tells the flipper to expect direct data
	GX_ClearVtxDesc();
	GX_InvVtxCache ();
	GX_InvalidateTexAll();

	GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);
	GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
	GX_SetVtxDesc (GX_VA_CLR0, GX_DIRECT);

	GX_SetVtxAttrFmt (GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GX_SetVtxAttrFmt (GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	GX_SetZMode (GX_FALSE, GX_LEQUAL, GX_TRUE);

	GX_SetNumChans(1);
	GX_SetNumTexGens(1);
	GX_SetTevOp (GX_TEVSTAGE0, GX_PASSCLR);
	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

	guMtxIdentity(GXmodelView2D);
	guMtxTransApply (GXmodelView2D, GXmodelView2D, 0.0F, 0.0F, -50.0F);
	GX_LoadPosMtxImm(GXmodelView2D,GX_PNMTX0);

	guOrtho(p,0,479,0,639,0,300);
	GX_LoadProjectionMtx(p, GX_ORTHOGRAPHIC);

	GX_SetViewport(0,0,_resolution->fbWidth,_resolution->efbHeight,0,1);
	GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
	GX_SetAlphaUpdate(GX_TRUE);
}

void VideoWII::stop( void)
{
	GX_AbortFrame();
	GX_Flush();

	VIDEO_SetBlack(TRUE);
	VIDEO_Flush();
}

#elif defined PSL1GHT
void VideoPS3::initializeGPU(void)
{
   // Allocate a 1Mb buffer, alligned to a 1Mb boundary to be our shared IO memory with the RSX.
    void *host_addr = memalign(1024*1024, 1024*1024);
    assert(host_addr != NULL);

    // Initilise Reality, which sets up the command buffer and shared IO memory
    _CommandBuffer = realityInit(0x10000, 1024*1024, host_addr);
    assert(_CommandBuffer != NULL);
}

void VideoPS3::setupScreenMode(void)
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

    s32 buffer_size = 4 * _resolution.width * _resolution.height; // each pixel is 4 bytes
    printf("buffers will be 0x%x bytes\n", buffer_size);

    gcmSetFlipMode(GCM_FLIP_VSYNC); // Wait for VSYNC to flip
}

void VideoPS3::initializeDoubleBuffer(void)
{
    // Allocate two buffers for the RSX to draw to the screen (double buffering)
    buffer[0] = rsxMemAlign(16, buffer_size);
    buffer[1] = rsxMemAlign(16, buffer_size);
    assert(buffer[0] != NULL && buffer[1] != NULL);

    u32 offset[2];
    assert(realityAddressToOffset(buffer[0], &offset[0]) == 0);
    assert(realityAddressToOffset(buffer[1], &offset[1]) == 0);
    // Setup the display buffers
    assert(gcmSetDisplayBuffer(0, offset[0], _resolution.width * 4, _resolution.width, _resolution.height) == 0);
    assert(gcmSetDisplayBuffer(1, offset[1], _resolution.width * 4, _resolution.width, _resolution.height) == 0);

    gcmResetFlipStatus();
}

void VideoPS3::flip( void)
{
    assert(gcmSetFlip(_CommandBuffer, _currentBuffer) == 0);
    realityFlushBuffer(_CommandBuffer);
    gcmSetWaitFlip(_CommandBuffer); // Prevent the RSX from continuing until the flip has finished.

	_currentBuffer ^= 1; // flip framebuffer
}

void VideoPS3::waitFlip( void)
{
	while(gcmGetFlipStatus() != 0)
        usleep(200);
    gcmResetFlipStatus();
}

void VideoPS3::clearBackground( void)
{
	s32 i, j;
    for(i = 0; i < res.height; i++) {
        s32 color = 0;
        for(j = 0; j < res.width; j++)
            buffer[i* res.width + j] = color;
    }

}

void VideoPS3::setupVertexDesc( void)
{
}

void VideoPS3::stop( void)
{

}
#endif

