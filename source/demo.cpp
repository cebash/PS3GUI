/****************************************************************************
 * libwiigui Template
 * Tantric 2009
 *
 * demo.cpp
 * Basic template/demonstration of libwiigui capabilities. For a
 * full-featured app using many more extensions, check out Snes9x GX.
 ***************************************************************************/

#ifdef DEVKITPPC
	#include <gccore.h>
	#include <ogcsys.h>
	#include <wiiuse/wpad.h>
	#include <fat.h>
#endif
#include "libps3gui/hal/inputpadmodule.h"
#include "libps3gui/hal/videomodule.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "video.h"
#include "input.h"
#ifdef DEVKITPPC
	#include "FreeTypeGX.h"
	#include "audio.h"
	#include "menu.h"
	#include "filelist.h"
#else
	#include <pngdec/pngdec.h>
	#include <pngdec/loadpng.h>
	#include <sysmodule/sysmodule.h>
#endif

#include "demo.h"

struct SSettings Settings;
int ExitRequested = 0;

#define DEBUG_X 800
#define DEBUG_Y 400


void drawFrame(unsigned int *buffer, long frame) {
	VideoModule * video = VideoModule::getVideoModule();
	u32 height = video->getHeight(), width = video->getWidth();
	u32 i, j;
	for(i = 0; i < height; i++) {
		s32 color = (i / (height * 1.0) * 256);
		// This should make a nice black to green graident
		color = (color << 8) | ((frame % 255) << 16);
		for(j = 0; j < width; j++)
			buffer[i* width + j] = color;
	}

}
void ExitApp()
{
#ifdef TOPORT
	ShutoffRumble();
#endif
	StopVideo();
	exit(0);
}

void
DefaultSettings()
{
	Settings.LoadMethod = METHOD_AUTO;
	Settings.SaveMethod = METHOD_AUTO;
	sprintf (Settings.Folder1,"libwiigui/first folder");
	sprintf (Settings.Folder2,"libwiigui/second folder");
	sprintf (Settings.Folder3,"libwiigui/third folder");
	Settings.AutoLoad = 1;
	Settings.AutoSave = 1;
}

#ifdef PSL1GHT

u32 module_flag;

void unload_modules(){

	VideoModule::getVideoModule()->stop();


	if(module_flag & 2)
		SysUnloadModule(SYSMODULE_PNGDEC);

	if(module_flag & 1)
		SysUnloadModule(SYSMODULE_FS);
}
#endif

s32 main(s32 argc, const char* argv[])
{
	atexit(unload_modules);

#ifdef TOPORT
	InitVideo(); // Initialize video
	SetupPads(); // Initialize input
	InitAudio(); // Initialize audio
	fatInitDefault(); // Initialize file system
	InitFreeType((u8*)font_ttf, font_ttf_size); // Initialize font system
	InitGUIThreads(); // Initialize GUI
	DefaultSettings();
	MainMenu(MENU_SETTINGS);
#endif

#ifdef PSL1GHT

	if(SysLoadModule(SYSMODULE_FS)!=0) return 0; else module_flag |=1;

	if(SysLoadModule(SYSMODULE_PNGDEC)!=0) return 0; else module_flag |=2;

#endif
	VideoModule * video = VideoModule::getVideoModule();
	video->init();
	video->initConsole();

	InputPadModule * inpad = InputPadModule::getInputPadModule();
	inpad->init(ePadAll);

#ifdef PSL1GHT
	PngDatas png1;

	LoadPNG(&png1, (char *)"/dev_usb/PS3_GAME/ICON0.PNG");
#endif


	long frame = 0; // To keep track of how many frames we have rendered.

	// Ok, everything is setup. Now for the main loop.
	while(1){
		char str [300];
		sprintf(str, "Pad number : %d, frame %d", (int) inpad->getPadNumber(),(int)frame);

		inpad->refreshPadConnections();
		// Check the pads.
		for( size_t i = 0; i < inpad->getPadNumber(); i++)
		{
			if( inpad->getPad(i)->isPressed( eAbsButExecute))
				return 0;
		}

		video->waitFlip(); // Wait for the last flip to finish, so we can draw to the old buffer

#ifdef PSL1GHT
		drawFrame(video->getCurrentBuffer(), frame++); // Draw into the unused buffer

		if(png1.bmp_out) {
			video->displayBitmap( (u32 *)png1.bmp_out, png1.height, png1.width, png1.wpitch);
		}
#endif
		video->printf(DEBUG_X, DEBUG_Y, str);

		video->flip(); // Flip buffer onto screen
	}

	return 0;
}

