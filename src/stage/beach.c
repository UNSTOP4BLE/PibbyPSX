/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "beach.h"

#include "../archive.h"
#include "../mem.h"
#include "../stage.h"

//Week 1 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	Gfx_Tex tex_beach; //bg
} Back_beach;

//Week 1 background functions
void Back_beach_DrawBG(StageBack *back)
{
	Back_beach *this = (Back_beach*)back;
	
	fixed_t fx, fy;

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
	if (stage.fade != 1)
		Stage_DrawTex(&this->tex_beach, &beach_src, &beach_dst, stage.camera.bzoom);
}

void Back_beach_Free(StageBack *back)
{
	Back_beach *this = (Back_beach*)back;
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_beach_New(void)
{
	//Allocate background structure
	Back_beach *this = (Back_beach*)Mem_Alloc(sizeof(Back_beach));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = NULL;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_beach_DrawBG;
	this->back.free = Back_beach_Free;
		
	Gfx_SetClear(0, 0, 0);

	//Load background textures
	IO_Data arc_back = IO_Read("\\BEACH\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_beach, Archive_Find(arc_back, "beach.tim"), 0);
	Mem_Free(arc_back);
	
	return (StageBack*)this;
}
