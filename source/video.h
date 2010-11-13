/****************************************************************************
 * libwiigui Template
 * Tantric 2009
 *
 * video.h
 * Video routines
 ***************************************************************************/

#ifndef _VIDEO_H_
#define _VIDEO_H_

#ifdef DEVKITPPC
#include <ogcsys.h>
#endif

#include "libps3gui/gui_types.h"

void InitVideo ();
void StopVideo();
void Menu_Render();

#ifdef DEVKITPPC
#pragma message "Port : "__FILE__" :"
#pragma message "ResetVideo_Menu"
void ResetVideo_Menu();
#pragma message "Menu_DrawImg"
void Menu_DrawImg(f32 xpos, f32 ypos, u16 width, u16 height, u8 data[], f32 degrees, f32 scaleX, f32 scaleY, u8 alphaF );
#pragma message "Menu_DrawRectangle"
void Menu_DrawRectangle(f32 x, f32 y, f32 width, f32 height, GXColor color, u8 filled);
#endif

//extern int screenheight;
//extern int screenwidth;
extern u32 FrameTimer;

#endif
