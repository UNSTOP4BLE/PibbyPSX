/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "finn.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//finn character structure
enum
{
	finn_ArcMain_Idle0,
	finn_ArcMain_Idle1,
	finn_ArcMain_Idle2,
	finn_ArcMain_Idle3,
	finn_ArcMain_Idle4,
	finn_ArcMain_Left0,
	finn_ArcMain_Left1,
	finn_ArcMain_Down0,
	finn_ArcMain_Down1,
	finn_ArcMain_Up0,
	finn_ArcMain_Up1,
	finn_ArcMain_Right0,
	finn_ArcMain_Right1,

	finn_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[finn_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_finn;

//finn character definitions
static const CharFrame char_finn_frame[] = {
	{finn_ArcMain_Idle0, {  0,   0, 138, 144}, {  0,   0}}, //0 idle 1
	{finn_ArcMain_Idle1, {  0,   0, 138, 144}, {  0,   0}}, //1 idle 2
	{finn_ArcMain_Idle2, {  0,   0, 134, 150}, {  0,   0}}, //2 idle 3
	{finn_ArcMain_Idle3, {  0,   0, 128, 157}, {  0,   0}}, //3 idle 4
	{finn_ArcMain_Idle4, {  0,   0, 124, 166}, {  0,   0}}, //4 idle 5

	{finn_ArcMain_Left0, {  0,   0, 176, 137}, {  0,   0}}, //6 left 1
	{finn_ArcMain_Left1, {  0,   0, 178, 150}, {  0,   0}}, //7 left 2
	
	{finn_ArcMain_Down0, {  0,   0, 142, 140}, {  0,   0}}, //8 down 1
	{finn_ArcMain_Down1, {  0,   0, 144, 153}, {  0,   0}}, //9 down 2
	
	{finn_ArcMain_Up0, {  0,   0, 182, 180}, {  0,   0}}, //10 up 1gud
	{finn_ArcMain_Up1, {  0,   0, 188, 175}, {  0,   0}}, //11 up 2 gud
	
	{finn_ArcMain_Right0, {  0,   0, 212, 154}, {  0,   0}}, //12 right 1 gud
	{finn_ArcMain_Right1, {  0,   0, 206, 156}, {  0,   0}}, //13 right 2 gud
};

static const Animation char_finn_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0, 1, 2, 3, 4, ASCR_BACK, 0}}, //CharAnim_Idle
	{2, (const u8[]){ 5, 6, ASCR_BACK, 0}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 7, 8, ASCR_BACK, 0}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 9, 10, ASCR_BACK, 0}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){ 11, 12, ASCR_BACK, 0}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//finn character functions
void Char_finn_SetFrame(void *user, u8 frame)
{
	Char_finn *this = (Char_finn*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_finn_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_finn_Tick(Character *character)
{
	Char_finn *this = (Char_finn*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_finn_SetFrame);
	Character_Draw(character, &this->tex, &char_finn_frame[this->frame]);
}

void Char_finn_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_finn_Free(Character *character)
{
	Char_finn *this = (Char_finn*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_finn_New(fixed_t x, fixed_t y)
{
	//Allocate finn object
	Char_finn *this = Mem_Alloc(sizeof(Char_finn));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_finn_New] Failed to allocate finn object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_finn_Tick;
	this->character.set_anim = Char_finn_SetAnim;
	this->character.free = Char_finn_Free;
	
	Animatable_Init(&this->character.animatable, char_finn_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 3;
	
	this->character.focus_x = FIXED_DEC(-142 - -120,1);
	this->character.focus_y = FIXED_DEC(25 - 120,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\FINN.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //finn_ArcMain_Idle0
		"idle1.tim", //finn_ArcMain_Idle1
		"idle2.tim", //finn_ArcMain_Idle1
		"idle3.tim", //finn_ArcMain_Idle1
		"idle4.tim", //finn_ArcMain_Idle1
		"left0.tim", //finn_ArcMain_Idle1
		"left1.tim", //finn_ArcMain_Idle1
		"down0.tim", //finn_ArcMain_Idle1
		"down1.tim", //finn_ArcMain_Idle1
		"up0.tim", //finn_ArcMain_Idle1
		"up1.tim", //finn_ArcMain_Idle1
		"right0.tim", //finn_ArcMain_Idle1
		"right1.tim", //finn_ArcMain_Idle1

		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}