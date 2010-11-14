

#include "videomodulewii.h"

#ifdef DEVKITPPC
#include <gccore.h>
#include <ogcsys.h>
#include <wiiuse/wpad.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef DEVKITPPC

void VideoModuleWII::setupScreenMode( void)
{
	VIDEO_Init();
	_resolution = VIDEO_GetPreferredMode(NULL); // get default video mode

	// widescreen fix
	if(CONF_GetAspectRatio() == CONF_ASPECT_16_9)
		_resolution->viWidth = VI_MAX_WIDTH_PAL;

	VIDEO_Configure (_resolution);

}


void VideoModuleWII::initializeDoubleBuffer( void)
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


void VideoModuleWII::initializeGPU( void)
{
	// Initialize GX
	GXColor background = { 0, 0, 0, 0xff };
	memset (&_CommandBuffer, 0, DEFAULT_FIFO_SIZE);
	GX_Init (&_CommandBuffer, DEFAULT_FIFO_SIZE);
	GX_SetCopyClear (background, 0x00ffffff);
	GX_SetDispCopyGamma (GX_GM_1_0);
	GX_SetCullMode (GX_CULL_NONE);
}


void VideoModuleWII::flip( void)
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

void VideoModuleWII::clearBackground( void)
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

void VideoModuleWII::setupVertexDesc( void)
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

void VideoModuleWII::stop( void)
{
	GX_AbortFrame();
	GX_Flush();

	VIDEO_SetBlack(TRUE);
	VIDEO_Flush();
}
#endif

