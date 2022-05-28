/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "bfmenu.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//bfmenu character structure
enum
{
	bfmenu_ArcMain_bfmenu,
	
	bfmenu_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[bfmenu_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_bfmenu;

//bfmenu character definitions
static const CharFrame char_bfmenu_frame[] = {
	{bfmenu_ArcMain_bfmenu, {  1,   1, 81, 79}, {119,  78}}, //0 idle 1
	{bfmenu_ArcMain_bfmenu, {  84,  1, 81, 80}, {113,  79}}, //1 idle 2
	{bfmenu_ArcMain_bfmenu, {  0,   0, 149, 207}, {113,  206}}, //2 idle 3
	{bfmenu_ArcMain_bfmenu, {  0,   0, 149, 208}, {112,  205}}, //3 idle 4
	{bfmenu_ArcMain_bfmenu, {  0,   0, 150, 209}, {115,  205}}, //4 idle 5

	{bfmenu_ArcMain_bfmenu, {  0,   0, 149, 208}, {114,  204}}, //5 idle 6
	{bfmenu_ArcMain_bfmenu, {  0,   0, 150, 206}, {114,  204}}, //6 idle 7
	{bfmenu_ArcMain_bfmenu, {  0,   0, 150, 205}, {113,  202}}, //7 idle 8
};

static const Animation char_bfmenu_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0, 1, 2, 3, 4, ASCR_BACK, 0}}, //CharAnim_Idle
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//bfmenu character functions
void Char_bfmenu_SetFrame(void *user, u8 frame)
{
	Char_bfmenu *this = (Char_bfmenu*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_bfmenu_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_bfmenu_Tick(Character *character)
{
	Char_bfmenu *this = (Char_bfmenu*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_bfmenu_SetFrame);
	Character_Draw(character, &this->tex, &char_bfmenu_frame[this->frame]);
}

void Char_bfmenu_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_bfmenu_Free(Character *character)
{
	Char_bfmenu *this = (Char_bfmenu*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_bfmenu_New(fixed_t x, fixed_t y)
{
	//Allocate bfmenu object
	Char_bfmenu *this = Mem_Alloc(sizeof(Char_bfmenu));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_bfmenu_New] Failed to allocate bfmenu object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_bfmenu_Tick;
	this->character.set_anim = Char_bfmenu_SetAnim;
	this->character.free = Char_bfmenu_Free;
	
	Animatable_Init(&this->character.animatable, char_bfmenu_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 4;
	
	this->character.focus_x = FIXED_DEC(-142 - -120,1);
	this->character.focus_y = FIXED_DEC(25 - 120,1);
	this->character.focus_zoom = FIXED_DEC(8,10);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\BFMENU.ARC;1");
	
	const char **pathp = (const char *[]){
		"bfmenu.tim", //bfmenu_ArcMain_Idle0
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
