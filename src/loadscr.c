/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "loadscr.h"

#include "gfx.h"
#include "timer.h"
#include "io.h"
#include "audio.h"
#include "trans.h"
#include "network.h"

//Loading screen functions
void LoadScr_Start(void)
{
	//Stop music and make sure frame has been drawn
	Audio_StopXA();
	Network_Process();
	Gfx_Flip();
	
	//Load loading screen texture
	Gfx_Tex loading_tex;
	RECT back_src = {0, 0, 179, 170};
	RECT back_dst = {(SCREEN_WIDTH - 320) >> 1, (SCREEN_HEIGHT - 240) >> 1, 320, 240};

    RECT loading_src = {41, 171, 137, 66};
	RECT loading_dst = {320 - 290, 240 - 66 - 20, 137 + 30, 66};


    Gfx_SetClear(0, 0, 0);
	Gfx_LoadTex(&loading_tex, IO_Read("\\MENU\\LOADING.TIM;1"), GFX_LOADTEX_FREE);
	Timer_Reset();
	
	//Draw loading screen and run transition
	Trans_Set();
	
	while (!Trans_Idle())
	{
		//Draw loading screen and end frame
		Timer_Tick();
		Trans_Tick();
		Gfx_DrawTex(&loading_tex, &loading_src, &loading_dst);
        Gfx_DrawTex(&loading_tex, &back_src, &back_dst);
		Network_Process();
		Gfx_Flip();
	}
	
	//Draw an extra frame to avoid double buffering issues
    Gfx_DrawTex(&loading_tex, &loading_src, &loading_dst);
	Gfx_DrawTex(&loading_tex, &back_src, &back_dst);
	Network_Process();
	Gfx_Flip();
}

void LoadScr_End(void)
{
	//Handle transition out
	Timer_Reset();
	Trans_Clear();
	Trans_Start();
	Gfx_DisableClear();
	while (!Trans_Tick())
	{
		Timer_Tick();
		Network_Process();
		Gfx_Flip();
	}
	Gfx_EnableClear();
}
