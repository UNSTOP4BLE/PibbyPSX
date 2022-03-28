/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "week3.h"

#include "../mem.h"
#include "../archive.h"
#include "../random.h"
#include "../timer.h"

#include "../pad.h"
int x, y, w, h;

//Week 3 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	Gfx_Tex tex_bg; //Buildings
	Gfx_Tex tex_fg; //idk
	Gfx_Tex tex_sky; //Sky
	
} Back_Week3;

void Back_Week3_DrawBG(StageBack *back)
{
	Back_Week3 *this = (Back_Week3*)back;
	
	fixed_t fx, fy;

	FntPrint("x%dy%dw%dh%d", x, y, w, h);

	if (pad_state.held & PAD_LEFT)
		x--;
	if (pad_state.held & PAD_RIGHT)
		x ++;
	if (pad_state.held & PAD_UP)
		y--;
	if (pad_state.held & PAD_DOWN)
		y++;

	if (pad_state.held & PAD_SQUARE)
		w--;
	if (pad_state.held & PAD_CIRCLE)
		w ++;
	if (pad_state.held & PAD_TRIANGLE)
		h--;
	if (pad_state.held & PAD_CROSS)
		h++;

	//Draw fg
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	RECT fg_src = {0, 0, 256, 256};
	RECT_FIXED fg_dst = {
		FIXED_DEC(-324,1) - fx,
		FIXED_DEC(-183,1) - fy,
		FIXED_DEC(571,1),
		FIXED_DEC(343,1)
	};
	
	Stage_DrawTex(&this->tex_fg, &fg_src, &fg_dst, stage.camera.bzoom);

	//Draw bg
	fx = stage.camera.x >> 1;
	fy = stage.camera.y >> 1;

	RECT bg_src = {0, 0, 256, 256};
	RECT_FIXED bg_dst = {
		FIXED_DEC(-264,1) - fx,
		FIXED_DEC(-141,1) - fy,
		FIXED_DEC(445,1),
		FIXED_DEC(255,1)
	};
	
	Stage_DrawTex(&this->tex_bg, &bg_src, &bg_dst, stage.camera.bzoom);

	//Draw sky
	fx = stage.camera.x >> 3;
	fy = stage.camera.y >> 3;
	
	RECT sky_src = {0, 0, 255, 128};
	RECT_FIXED sky_dst = {
		FIXED_DEC(-175,1) - FIXED_DEC(SCREEN_WIDEOADD,2) - fx,
		FIXED_DEC(-121,1) - FIXED_DEC(SCREEN_WIDEOADD,4) - fy,
		FIXED_DEC(185,1) + FIXED_DEC(SCREEN_WIDEOADD,1),
		FIXED_DEC(120,1) + FIXED_DEC(SCREEN_WIDEOADD,2)
	};
	
	Stage_DrawTex(&this->tex_sky, &sky_src, &sky_dst, stage.camera.bzoom);
	sky_dst.x += sky_dst.w;
	sky_src.y += sky_src.h;
	Stage_DrawTex(&this->tex_sky, &sky_src, &sky_dst, stage.camera.bzoom);
}

void Back_Week3_Free(StageBack *back)
{
	Back_Week3 *this = (Back_Week3*)back;
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_Week3_New(void)
{
	//Allocate background structure
	Back_Week3 *this = (Back_Week3*)Mem_Alloc(sizeof(Back_Week3));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = NULL;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Week3_DrawBG;
	this->back.free = Back_Week3_Free;
	
	Gfx_SetClear(0, 0, 0);

	//Load background textures
	IO_Data arc_back = IO_Read("\\WEEK3\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_sky, Archive_Find(arc_back, "sky.tim"), 0);
	Gfx_LoadTex(&this->tex_bg, Archive_Find(arc_back, "bg.tim"), 0);
	Gfx_LoadTex(&this->tex_fg, Archive_Find(arc_back, "fg.tim"), 0);
	Mem_Free(arc_back);

	return (StageBack*)this;
}