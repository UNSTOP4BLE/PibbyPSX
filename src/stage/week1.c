/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "week1.h"

#include "../archive.h"
#include "../mem.h"
#include "../stage.h"
/*
#include "../pad.h"
int x, y, w, h;
*/
//Week 1 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	Gfx_Tex tex_beach; //bg
} Back_Week1;

//Week 1 background functions
void Back_Week1_DrawBG(StageBack *back)
{
	Back_Week1 *this = (Back_Week1*)back;
	
	fixed_t fx, fy;

/*
	FntPrint("x%d y%d w%d h%d", x, y, w, h);
	
	if (pad_state.held & PAD_L1)
		x ++;
	if (pad_state.held & PAD_R1)
		x --;
	if (pad_state.held & PAD_L2)
		y ++;
	if (pad_state.held & PAD_R2)
		y --;
	if (pad_state.held & PAD_LEFT)
		w ++;
	if (pad_state.held & PAD_RIGHT)
		w --;
	if (pad_state.held & PAD_SQUARE)
		h ++;
	if (pad_state.held & PAD_CIRCLE)
		h --;
*/

	//Draw bg
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	RECT beach_src = {0, 0, 256, 256};
	RECT_FIXED beach_dst = {
		FIXED_DEC(-390,1) - fx,
		FIXED_DEC(-131,1) - fy,
		FIXED_DEC(700,1),
		FIXED_DEC(320,1)
	};
	Stage_DrawTex(&this->tex_beach, &beach_src, &beach_dst, stage.camera.bzoom);
}

void Back_Week1_Free(StageBack *back)
{
	Back_Week1 *this = (Back_Week1*)back;
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_Week1_New(void)
{
	//Allocate background structure
	Back_Week1 *this = (Back_Week1*)Mem_Alloc(sizeof(Back_Week1));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = NULL;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Week1_DrawBG;
	this->back.free = Back_Week1_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\WEEK1\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_beach, Archive_Find(arc_back, "beach.tim"), 0);
	Mem_Free(arc_back);
	
	return (StageBack*)this;
}
