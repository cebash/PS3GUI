/****************************************************************************
 * libwiigui Template
 * Tantric 2009
 *
 * input.h
 * Wii/GameCube controller management
 ***************************************************************************/

#ifndef _INPUT_H_
#define _INPUT_H_

#ifdef DEVKITPPC
#include <gccore.h>
#include <wiiuse/wpad.h>
#endif

#define PI 				3.14159265f
#define PADCAL			50

extern int rumbleRequest[4];

void SetupPads();
void ShutoffRumble();
#ifdef DEVKITPPC
void UpdatePads();
void DoRumble(int i);
#endif

#endif

