
/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "week2.h"

#include "../mem.h"
#include "../archive.h"


//#include "../pad.h"
//int x, y, w, h;
//Week 2 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	Gfx_Tex tex_back0; //Background

} Back_Week2;

//Week 2 background functions
void Back_Week2_DrawBG(StageBack *back)
{
	Back_Week2 *this = (Back_Week2*)back;
	
	fixed_t fx, fy;
	fx = stage.camera.x;
	fy = stage.camera.y;
/*	
	FntPrint("x%d y%d w%d h%d", x, y, w, h);

	if (pad_state.held & PAD_LEFT)
		x++;
	if (pad_state.held & PAD_RIGHT)
		x--;
	if (pad_state.held & PAD_UP)
		y++;
	if (pad_state.held & PAD_DOWN)
		y--;

	if (pad_state.held & PAD_SQUARE)
		w++;
	if (pad_state.held & PAD_CIRCLE)
		w--;
	if (pad_state.held & PAD_TRIANGLE)
		h++;
	if (pad_state.held & PAD_CROSS)
		h--;
 */
	//Draw background
	RECT back_src = {0, 0, 256, 256};
	RECT_FIXED back_dst = {
		FIXED_DEC(-227,1) - fx,
		FIXED_DEC(-111,1) - fy,
		FIXED_DEC(523,1),
		FIXED_DEC(304,1)
	};
	
	Stage_DrawTex(&this->tex_back0, &back_src, &back_dst, stage.camera.bzoom);
}

void Back_Week2_Free(StageBack *back)
{
	Back_Week2 *this = (Back_Week2*)back;
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_Week2_New(void)
{
	//Allocate background structure
	Back_Week2 *this = (Back_Week2*)Mem_Alloc(sizeof(Back_Week2));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = NULL;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Week2_DrawBG;
	this->back.free = Back_Week2_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\WEEK2\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back0.tim"), 0);
	Mem_Free(arc_back);
	
	return (StageBack*)this;
}
