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
#endif
#include "demo.h"

struct SSettings Settings;
int ExitRequested = 0;

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

int
main(int argc, char *argv[])
{
	printf( "InitVideo\n");
	InitVideo(); // Initialize video
	SetupPads(); // Initialize input
#ifdef TOPORT
	InitAudio(); // Initialize audio
	fatInitDefault(); // Initialize file system
	InitFreeType((u8*)font_ttf, font_ttf_size); // Initialize font system
	InitGUIThreads(); // Initialize GUI
#endif

	DefaultSettings();
#ifdef TOPORT
	MainMenu(MENU_SETTINGS);
#endif
}
