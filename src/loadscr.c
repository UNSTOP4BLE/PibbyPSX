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
#include "stage.h"

s16 test;
//Loading screen functions
void LoadScr_Start(void)
{
	//Stop music and make sure frame has been drawn
	Audio_StopXA();
	Network_Process();
	Gfx_Flip();
		
		//Draw boppers
	static const struct pibby
	{
		RECT src;
		RECT dst;
	} lbop_piece[] = {
		{{179, 170, 76, 85},  {10, 60, 76, 85}},
	};
	
	//Load loading screen texture
	Gfx_Tex loading_tex;

    Gfx_SetClear(0, 0, 0);
	Gfx_LoadTex(&loading_tex, IO_Read("\\MENU\\LOADING.TIM;1"), GFX_LOADTEX_FREE);
	Timer_Reset();
	
	//Draw loading screen and run transition
	Trans_Set();
	
	while (!Trans_Idle())
	{
		test++;
	stage.song_step = test /20;
	
	RECT back_src = {0, 0, 179, 170};
	RECT back_dst = {(SCREEN_WIDTH - 320) >> 1, (SCREEN_HEIGHT - 240) >> 1, 320, 240};

    RECT loading_src = {41, 171, 137, 66};
	RECT loading_dst = {320 - 290 + test, 240 - 66 - 20, 137 + 30, 66};
	
		//Draw loading screen and end frame
		Timer_Tick();
		Trans_Tick();
		
         fixed_t beat_bop;
	if ((stage.song_step & 0x3) == 0)
		beat_bop = FIXED_UNIT >> FIXED_SHIFT;
	else
		beat_bop = 0;
		
         const struct pibby *lbop_p = lbop_piece;
	for (size_t i = 0; i < COUNT_OF(lbop_piece); i++, lbop_p++)
	{
		RECT lbop_dst = {
			lbop_p->dst.x - (beat_bop << 1),
			lbop_p->dst.y + (beat_bop << 3),
			lbop_p->dst.w + (beat_bop << 2),
			lbop_p->dst.h - (beat_bop << 3),
		};
			Gfx_DrawTex(&loading_tex, &lbop_p->src, &lbop_dst);
	}
		Gfx_DrawTex(&loading_tex, &loading_src, &loading_dst);
        Gfx_DrawTex(&loading_tex, &back_src, &back_dst);
		Network_Process();
		Gfx_Flip();
	}
	
	//Draw an extra frame to avoid double buffering issues
	
	RECT back_src = {0, 0, 179, 170};
	RECT back_dst = {(SCREEN_WIDTH - 320) >> 1, (SCREEN_HEIGHT - 240) >> 1, 320, 240};

    RECT loading_src = {41, 171, 137, 66};
	RECT loading_dst = {320 - 290 + test, 240 - 66 - 20, 137 + 30, 66};
	
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
