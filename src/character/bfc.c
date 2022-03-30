/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "bfc.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//bfc character structure
enum
{
	bfc_ArcMain_Idle0,
	bfc_ArcMain_Idle1,
	bfc_ArcMain_Left,
	bfc_ArcMain_Down,
	bfc_ArcMain_Up,
	bfc_ArcMain_Right,

	bfc_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[bfc_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_bfc;

//bfc character definitions
static const CharFrame char_bfc_frame[] = {
	{bfc_ArcMain_Idle0, {  0,   0,  0,  0}, { 0,   0}}, //0 idle 1
	{bfc_ArcMain_Idle0, {  0,   0,  0,  0}, { 0,   0}}, //1 idle 2
	{bfc_ArcMain_Idle0, {  0,   0,  0,  0}, { 0,   0}}, //2 idle 3
	{bfc_ArcMain_Idle0, {  0,   0,  0,  0}, { 0,   0}}, //3 idle 4
	{bfc_ArcMain_Idle1, {  0,   0,  0,  0}, { 0,   0}}, //4 idle 5
	{bfc_ArcMain_Idle1, {  0,   0,  0,  0}, { 0,   0}}, //5 idle 6
	{bfc_ArcMain_Idle1, {  0,   0,  0,  0}, { 0,   0}}, //6 idle 7

	{bfc_ArcMain_Left, {  0,   0,  0,  0}, { 0,   0}}, //4 left 1
	{bfc_ArcMain_Left, {  0,   0,  0,  0}, { 0,   0}}, //5 left 2
	{bfc_ArcMain_Left, {  0,   0,  0,  0}, { 0,   0}}, //4 left 1
	{bfc_ArcMain_Left, {  0,   0,  0,  0}, { 0,   0}}, //5 left 2

	{bfc_ArcMain_Down, {  0,   0,  94,  92}, { 89,   84}}, //6 down 1
	{bfc_ArcMain_Down, { 94,   0,  94,  94}, { 88,   85}}, //7 down 2
	{bfc_ArcMain_Down, {  0,  92,  94,  96}, { 87,   85}}, //6 down 1
	{bfc_ArcMain_Down, { 94,  94,  95,  95}, { 88,   85}}, //7 down 2

	{bfc_ArcMain_Up, {  0,   0,  93,  113}, { 92,   104}}, //8 up 1
	{bfc_ArcMain_Up, { 93,   0,  94,  112}, { 93,   103}}, //9 up 2
	{bfc_ArcMain_Up, {  0, 113,  95,  114}, { 93,   103}}, //8 up 1
	{bfc_ArcMain_Up, { 95, 112,  94,  113}, { 92,   113}}, //9 up 2
	
	{bfc_ArcMain_Right, {  0,   0,  0,  0}, { 0,   0}}, //10 right 1
	{bfc_ArcMain_Right, {  0,   0,  0,  0}, { 0,   0}}, //11 right 2
	{bfc_ArcMain_Right, {  0,   0,  0,  0}, { 0,   0}}, //10 right 1
	{bfc_ArcMain_Right, {  0,   0,  0,  0}, { 0,   0}}, //11 right 2
};

static const Animation char_bfc_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0, 1, 2, 3, 4, 5, 6, ASCR_BACK, 0}}, //CharAnim_Idle
	{2, (const u8[]){ 7, 8, 9, 10, ASCR_BACK, 0}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 11, 12, 13, 14, ASCR_BACK, 0}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 15, 16, 17, 18, ASCR_BACK, 0}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){ 19, 20, 21, 22 , ASCR_BACK, 0}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//bfc character functions
void Char_bfc_SetFrame(void *user, u8 frame)
{
	Char_bfc *this = (Char_bfc*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_bfc_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_bfc_Tick(Character *character)
{
	Char_bfc *this = (Char_bfc*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_bfc_SetFrame);
	Character_Draw(character, &this->tex, &char_bfc_frame[this->frame]);
}

void Char_bfc_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_bfc_Free(Character *character)
{
	Char_bfc *this = (Char_bfc*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_bfc_New(fixed_t x, fixed_t y)
{
	//Allocate bfc object
	Char_bfc *this = Mem_Alloc(sizeof(Char_bfc));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_bfc_New] Failed to allocate bfc object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_bfc_Tick;
	this->character.set_anim = Char_bfc_SetAnim;
	this->character.free = Char_bfc_Free;
	
	Animatable_Init(&this->character.animatable, char_bfc_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 3;
	
	this->character.focus_x = FIXED_DEC(-142 - -120,1);
	this->character.focus_y = FIXED_DEC(25 - 120,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\BFC.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //bfc_ArcMain_Idle0
		"idle1.tim", //bfc_ArcMain_Idle1
		"left.tim",
		"down.tim",
		"up.tim",
		"right.tim",
		
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
