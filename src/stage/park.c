/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "park.h"

#include "../mem.h"
#include "../archive.h"

//park background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	Gfx_Tex tex_park; //Background
} Back_park;

//park background functions
void Back_park_DrawBG(StageBack *back)
{
	Back_park *this = (Back_park*)back;
	
	fixed_t fx, fy;
	fx = stage.camera.x;
	fy = stage.camera.y;

	//Draw background
	RECT back_src = {0, 0, 256, 256};
	RECT_FIXED back_dst = {
		FIXED_DEC(-314,1) - fx,
		FIXED_DEC(-166,1) - fy,
		FIXED_DEC(667,1),
		FIXED_DEC(318,1)
	};
	
	if (stage.fade == 0)
		Stage_DrawTex(&this->tex_park, &back_src, &back_dst, stage.camera.bzoom);
}

void Back_park_Free(StageBack *back)
{
	Back_park *this = (Back_park*)back;
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_park_New(void)
{
	//Allocate background structure
	Back_park *this = (Back_park*)Mem_Alloc(sizeof(Back_park));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = NULL;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_park_DrawBG;
	this->back.free = Back_park_Free;
	
	Gfx_SetClear(0, 0, 0);
	//Load background textures
	IO_Data arc_back = IO_Read("\\PARK\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_park, Archive_Find(arc_back, "park.tim"), 0);
	Mem_Free(arc_back);
	
	return (StageBack*)this;
}
