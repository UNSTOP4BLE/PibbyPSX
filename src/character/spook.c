/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "spook.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//Spook character structure
enum
{
	Spook_ArcMain_Idle0,
	Spook_ArcMain_Idle1,
	Spook_ArcMain_Hit0,
	Spook_ArcMain_Hit1,
	Spook_ArcMain_Hit2,

	Spook_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Spook_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Spook;

//Spook character definitions
static const CharFrame char_spook_frame[] = {
	{Spook_ArcMain_Idle0, {  0,   0,  95, 137}, { 54, 137}}, //0 idle 1
	{Spook_ArcMain_Idle0, { 95,   0,  95, 135}, { 53, 135}}, //1 idle 2
	{Spook_ArcMain_Idle0, { 86, 135,  91, 121}, { 52, 121}}, //2 idle 3
	{Spook_ArcMain_Idle1, {  0,   0,  88, 123}, { 51, 123}}, //3 idle 4
	{Spook_ArcMain_Idle1, {  0, 123, 126, 133}, { 69, 131}}, //4 idle 5
	{Spook_ArcMain_Idle1, {127, 123, 120, 131}, { 64, 130}}, //5 idle 6
	//gud
	
	{Spook_ArcMain_Hit1, {  0,   0, 139, 132}, {101, 131}}, //6 left 1
	{Spook_ArcMain_Hit1, {110, 123, 140, 133}, { 99, 132}}, //7 left 2
	
	{Spook_ArcMain_Hit0, {  0,   0, 116, 102}, { 50,  100}}, //8 down 1
	{Spook_ArcMain_Hit0, {116,   0, 112, 102}, { 52,  101}}, //9 down 2
	
	{Spook_ArcMain_Hit2, {  0,   0,  99, 143}, { 66, 143}}, //10 up 1gud
	{Spook_ArcMain_Hit2, { 99,   0, 100, 141}, { 65, 141}}, //11 up 2 gud
	
	{Spook_ArcMain_Hit0, {  0, 102, 110, 131}, {38, 130}}, //12 right 1 gud
	{Spook_ArcMain_Hit0, {110, 102, 109, 133}, {35, 133}}, //13 right 2 gud
};

static const Animation char_spook_anim[CharAnim_Max] = {
	{2, (const u8[]){ASCR_CHGANI, CharAnim_LeftAlt}}, //CharAnim_Idle
	{2, (const u8[]){ 6,  7, ASCR_BACK, 1}},          //CharAnim_Left
	{2, (const u8[]){ 0,  1,  2,  3, ASCR_BACK, 1}},  //CharAnim_LeftAlt
	{2, (const u8[]){ 8,  9, ASCR_BACK, 1}},          //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_LeftAlt}}, //CharAnim_DownAlt
	{2, (const u8[]){10, 11, ASCR_BACK, 1}},          //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_LeftAlt}}, //CharAnim_UpAlt
	{2, (const u8[]){12, 13, ASCR_BACK, 1}},          //CharAnim_Right
	{2, (const u8[]){ 4,  5,  2,  3, ASCR_BACK, 1}},  //CharAnim_RightAlt
};

//Spook character functions
void Char_Spook_SetFrame(void *user, u8 frame)
{
	Char_Spook *this = (Char_Spook*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_spook_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Spook_Tick(Character *character)
{
	Char_Spook *this = (Char_Spook*)character;
	
	//Perform spooky dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
	{
		Character_CheckEndSing(character);
		
		if (stage.flag & STAGE_FLAG_JUST_STEP)
		{
			if ((Animatable_Ended(&character->animatable) || character->animatable.anim == CharAnim_LeftAlt || character->animatable.anim == CharAnim_RightAlt) &&
				(character->animatable.anim != CharAnim_Left &&
				 character->animatable.anim != CharAnim_Down &&
				 character->animatable.anim != CharAnim_Up &&
				 character->animatable.anim != CharAnim_Right) &&
				(stage.song_step & 0x3) == 0)
				character->set_anim(character, CharAnim_Idle);
		}
	}
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Spook_SetFrame);
	Character_Draw(character, &this->tex, &char_spook_frame[this->frame]);
}

void Char_Spook_SetAnim(Character *character, u8 anim)
{
	//Set animation
	if (anim == CharAnim_Idle)
	{
		if (character->animatable.anim == CharAnim_LeftAlt)
			anim = CharAnim_RightAlt;
		else
			anim = CharAnim_LeftAlt;
		character->sing_end = FIXED_DEC(0x7FFF,1);
	}
	else
	{
		Character_CheckStartSing(character);
	}
	Animatable_SetAnim(&character->animatable, anim);
}

void Char_Spook_Free(Character *character)
{
	Char_Spook *this = (Char_Spook*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Spook_New(fixed_t x, fixed_t y)
{
	//Allocate spook object
	Char_Spook *this = Mem_Alloc(sizeof(Char_Spook));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Spook_New] Failed to allocate spook object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Spook_Tick;
	this->character.set_anim = Char_Spook_SetAnim;
	this->character.free = Char_Spook_Free;
	
	Animatable_Init(&this->character.animatable, char_spook_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 9;
	
	this->character.focus_x =  FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-80,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\SPOOK.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //Spook_ArcMain_Idle0
		"idle1.tim", //Spook_ArcMain_Idle1
		"hit0.tim",  //Spook_ArcMain_Left
		"hit1.tim",  //Spook_ArcMain_Down
		"hit2.tim",    //Spook_ArcMain_Up
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}